#include "BLEDevice.h"
#include "BLEClient.h"
#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"

// LED 핀 설정 (ESP32-S3)
#define LED_PIN_RED   25
#define LED_PIN_GREEN 26 
#define LED_PIN_BLUE  27

// PWM 설정
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

// BLE 서비스 및 특성 UUID (비콘과 동일)
#define SERVICE_UUID        "12345678-1234-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID "87654321-4321-1000-8000-00805F9B34FB"

// 타겟 디바이스
#define TARGET_DEVICE_NAME "ESP32-SmartKey"

// BLE 객체들
BLEScan* pBLEScan;
BLEClient* pClient = nullptr;
BLERemoteCharacteristic* pRemoteCharacteristic = nullptr;
BLEAddress* pServerAddress = nullptr;

// 연결 상태
bool doConnect = false;
bool connected = false;
bool doScan = false;
bool welcomeLightTriggered = false; // 🔥 웰컴라이트 최초 1회 플래그

// 성능 카운터
unsigned long scanCount = 0;
unsigned long unlockEventCount = 0;
unsigned long welcomeEventCount = 0;
unsigned long lastPerformanceReport = 0;

// 마지막 이벤트 시간
unsigned long lastUnlockTime = 0;
unsigned long lastWelcomeTime = 0;
const unsigned long EVENT_COOLDOWN = 2000; // 2초 쿨다운

// 함수 선언
bool connectToServer();  // bool 타입으로 수정
void triggerCarUnlockSequence();
void triggerWelcomeLight(int rssi);
void triggerConnectionWelcomeLight(); // 🔥 연결 시 웰컴라이트
void crazyRainbowFlash();
void smoothRainbowWave();
void elegantFadeInOut();
void instantCarUnlockSequence();
void setLEDs(int r, int g, int b);
void turnOffAllLEDs();
void testLEDs();
void hsvToRgb(int h, int s, int v, int* r, int* g, int* b);
void printPerformanceStats();
String getDistanceRange(int rssi);

// 알림 콜백
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  String receivedData = "";
  for (int i = 0; i < length; i++) {
    receivedData += (char)pData[i];
  }
  
  Serial.printf("📨 받은 데이터: %s\n", receivedData.c_str());
  
  if (receivedData == "UNLOCK") {
    unsigned long currentTime = millis();
    if (currentTime - lastUnlockTime > EVENT_COOLDOWN) {
      Serial.println("🔥🔥🔥 차량 해제 신호 수신! 🔥🔥🔥");
      triggerCarUnlockSequence();
      lastUnlockTime = currentTime;
      unlockEventCount++;
    }
  }
}

// 클라이언트 콜백
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    Serial.println("🔗 서버에 연결됨!");
    
    // 🔥 연결 시 최초 1회 웰컴라이트 실행!
    if (!welcomeLightTriggered) {
      Serial.println("✨🎉 최초 연결 웰컴라이트! 🎉✨");
      triggerConnectionWelcomeLight();
      welcomeLightTriggered = true;
      welcomeEventCount++;
    }
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    welcomeLightTriggered = false; // 🔥 연결 해제 시 웰컴라이트 플래그 리셋
    Serial.println("🔌 서버 연결 끊어짐 - 재스캔 시작");
    doScan = true;
  }
};

// 광고 디바이스 콜백
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String deviceName = advertisedDevice.getName().c_str();
    int rssi = advertisedDevice.getRSSI();
    
    // 우리 타겟 디바이스인지 확인
    if (deviceName == TARGET_DEVICE_NAME) {
      Serial.printf("🎯 타겟 디바이스 발견: %s (RSSI: %d)\n", deviceName.c_str(), rssi);
      
      // 연결되지 않았다면 연결 준비
      if (!connected && !doConnect) {
        pServerAddress = new BLEAddress(advertisedDevice.getAddress());
        doConnect = true;
        doScan = false;
        Serial.println("📡 연결 준비 완료!");
      }
      
      // 🔥 웰컴라이트는 스캔에서 제거 (연결 시에만 실행)
    }
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=========================================");
  Serial.println("🚗 ESP32-S3 Scanner (BLE 통신 방식)");
  Serial.println("=========================================");
  
  // LED 초기화
  Serial.println("💡 LED PWM 초기화...");
  ledcAttach(LED_PIN_RED, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(LED_PIN_GREEN, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(LED_PIN_BLUE, PWM_FREQ, PWM_RESOLUTION);
  
  // LED 테스트
  testLEDs();
  
  // BLE 초기화
  Serial.println("📡 BLE 클라이언트 초기화...");
  BLEDevice::init("ESP32-Scanner");
  
  // 스캔 설정
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  
  doScan = true;
  
  Serial.println("✅ 시스템 준비 완료!");
  Serial.println("🔍 타겟 디바이스 스캔 시작...");
  Serial.println("=========================================");
}

void loop() {
  // 연결 처리
  if (doConnect) {
    if (connectToServer()) {
      Serial.println("✅ 서버 연결 및 설정 완료!");
    } else {
      Serial.println("❌ 서버 연결 실패");
    }
    doConnect = false;
  }

  // 스캔 처리
  if (doScan) {
    scanCount++;
    Serial.printf("🔍 스캔 #%lu 시작...\n", scanCount);
    
    BLEScanResults* foundDevices = pBLEScan->start(3, false);  // 포인터로 수정
    pBLEScan->clearResults();
    
    if (!connected) {
      Serial.println("⏳ 연결 대기 중...");
    }
  }
  
  // 성능 통계
  printPerformanceStats();
  
  delay(1000);
}

// 서버 연결
bool connectToServer() {
  Serial.printf("🔗 서버 연결 시도: %s\n", pServerAddress->toString().c_str());
  
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
  
  // 연결
  if (!pClient->connect(*pServerAddress)) {
    Serial.println("❌ 연결 실패");
    return false;
  }
  Serial.println("✅ 연결 성공");
  
  // 서비스 찾기
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) {
    Serial.println("❌ 서비스를 찾을 수 없음");
    pClient->disconnect();
    return false;
  }
  Serial.println("✅ 서비스 발견");
  
  // 특성 찾기
  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.println("❌ 특성을 찾을 수 없음");
    pClient->disconnect();
    return false;
  }
  Serial.println("✅ 특성 발견");
  
  // 알림 등록
  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
    Serial.println("✅ 알림 등록 완료");
  }
  
  connected = true;
  return true;
}

// 차량 해제 시퀀스
void triggerCarUnlockSequence() {
  Serial.println("🚗⚡ 차량 해제 시퀀스 실행!");
  instantCarUnlockSequence();
}

// 웰컴라이트 트리거 (RSSI 기반 - 사용 안함)
void triggerWelcomeLight(int rssi) {
  if (rssi > -50) {
    crazyRainbowFlash();
  } else if (rssi > -65) {
    smoothRainbowWave();
  } else {
    elegantFadeInOut();
  }
}

// 🔥 연결 시 웰컴라이트 (최초 1회)
void triggerConnectionWelcomeLight() {
  Serial.println("🌟 연결 성공 웰컴라이트 시작!");
  
  // 특별한 연결 환영 효과 (더 화려하게)
  for (int round = 0; round < 2; round++) {
    // 무지개 웨이브
    for (int hue = 0; hue < 360; hue += 20) {
      int r, g, b;
      hsvToRgb(hue, 255, 150, &r, &g, &b);
      setLEDs(r, g, b);
      delay(40);
    }
  }
  
  // 마지막 골든 플래시
  for (int i = 0; i < 3; i++) {
    setLEDs(255, 215, 0); // 골드
    delay(200);
    turnOffAllLEDs();
    delay(150);
  }
  
  // 부드러운 페이드 아웃
  for (int brightness = 200; brightness >= 0; brightness -= 10) {
    setLEDs((255 * brightness) / 200, 
            (215 * brightness) / 200, 
            0);
    delay(30);
  }
  
  turnOffAllLEDs();
  Serial.println("✨ 연결 웰컴라이트 완료!");
}

// 거리 범위
String getDistanceRange(int rssi) {
  if (rssi > -40) return "매우 가까움";
  else if (rssi > -60) return "가까움";
  else if (rssi > -80) return "보통";
  else return "멀음";
}

// ========== LED 효과 함수들 ==========

void instantCarUnlockSequence() {
  // 화이트 플래시
  for (int i = 0; i < 3; i++) {
    setLEDs(255, 255, 255);
    delay(150);
    turnOffAllLEDs();
    delay(100);
  }
  
  turnOffAllLEDs();
  Serial.println("✅ 차량 해제 완료!");
}

void crazyRainbowFlash() {
  for (int i = 0; i < 10; i++) {
    setLEDs(random(0, 256), random(0, 256), random(0, 256));
    delay(80);
    turnOffAllLEDs();
    delay(50);
  }
  
  // 마지막 화이트 플래시
  setLEDs(255, 255, 255);
  delay(300);
  turnOffAllLEDs();
}

void smoothRainbowWave() {
  for (int hue = 0; hue < 360; hue += 15) {
    int r, g, b;
    hsvToRgb(hue, 255, 120, &r, &g, &b);
    setLEDs(r, g, b);
    delay(30);
  }
  turnOffAllLEDs();
}

void elegantFadeInOut() {
  int goldR = 255, goldG = 215, goldB = 0;
  
  for (int brightness = 0; brightness <= 80; brightness += 10) {
    setLEDs((goldR * brightness) / 100,
            (goldG * brightness) / 100,
            (goldB * brightness) / 100);
    delay(50);
  }
  
  delay(500);
  
  for (int brightness = 80; brightness >= 0; brightness -= 10) {
    setLEDs((goldR * brightness) / 100,
            (goldG * brightness) / 100,
            (goldB * brightness) / 100);
    delay(50);
  }
}

// ========== 유틸리티 함수들 ==========

void setLEDs(int r, int g, int b) {
  ledcWrite(LED_PIN_RED, r);
  ledcWrite(LED_PIN_GREEN, g);
  ledcWrite(LED_PIN_BLUE, b);
}

void turnOffAllLEDs() {
  ledcWrite(LED_PIN_RED, 0);
  ledcWrite(LED_PIN_GREEN, 0);
  ledcWrite(LED_PIN_BLUE, 0);
}

void testLEDs() {
  Serial.println("🔬 LED 테스트...");
  setLEDs(255, 0, 0); delay(300);
  setLEDs(0, 255, 0); delay(300);
  setLEDs(0, 0, 255); delay(300);
  setLEDs(255, 255, 255); delay(300);
  turnOffAllLEDs();
  Serial.println("✅ LED 테스트 완료!");
}

void printPerformanceStats() {
  unsigned long currentTime = millis();
  if (currentTime - lastPerformanceReport > 10000) {
    lastPerformanceReport = currentTime;
    
    Serial.println("📊 성능 통계 (10초간):");
    Serial.printf("   스캔 횟수: %lu\n", scanCount);
    Serial.printf("   해제 이벤트: %lu\n", unlockEventCount);
    Serial.printf("   웰컴라이트: %lu\n", welcomeEventCount);
    Serial.printf("   연결 상태: %s\n", connected ? "연결됨 ✅" : "스캔 중 🔍");
    Serial.printf("   메모리: %d bytes\n", ESP.getFreeHeap());
    Serial.println("🎯 대상: ESP32-SmartKey");
    Serial.println("----------------------------------------");
    
    scanCount = 0;
  }
}

void hsvToRgb(int h, int s, int v, int* r, int* g, int* b) {
  int c = (v * s) / 255;
  int x = c * (60 - abs((h / 60) % 2 * 60 - 60)) / 60;
  int m = v - c;
  
  if (h < 60) {
    *r = c; *g = x; *b = 0;
  } else if (h < 120) {
    *r = x; *g = c; *b = 0;
  } else if (h < 180) {
    *r = 0; *g = c; *b = x;
  } else if (h < 240) {
    *r = 0; *g = x; *b = c;
  } else if (h < 300) {
    *r = x; *g = 0; *b = c;
  } else {
    *r = c; *g = 0; *b = x;
  }
  
  *r += m; *g += m; *b += m;
}