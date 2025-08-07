#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"

// 하드웨어 핀 설정
#define STATUS_LED 48     // ESP32-S3 내장 LED
#define BUTTON_PIN 0      // BOOT 버튼
#define BUZZER_PIN 17     // 부저 핀
#define UNLOCK_LED_PIN 18 // 차량 해제 LED 핀

// BLE 서비스 및 특성 UUID
#define SERVICE_UUID        "12345678-1234-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID "87654321-4321-1000-8000-00805F9B34FB"

// BLE 객체들
BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;

// 버튼 상태 관리
bool lastButtonState = HIGH;
bool currentButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// 상태 관리
bool unlockEventActive = false;
unsigned long unlockEventStartTime = 0;
const unsigned long UNLOCK_EVENT_DURATION = 3000; // 3초

// BLE 서버 콜백
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("🔗 클라이언트 연결됨!");
    }

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("🔌 클라이언트 연결 해제됨");
      // 재광고 시작
      BLEDevice::startAdvertising();
      Serial.println("📡 재광고 시작");
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("======================================");
  Serial.println("🚗 ESP32-S3 Smart Key (BLE 통신 방식)");
  Serial.println("======================================");

  // 핀 초기화
  pinMode(STATUS_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(UNLOCK_LED_PIN, OUTPUT);
  
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(UNLOCK_LED_PIN, LOW);

  // 시작 신호
  for(int i = 0; i < 3; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(150);
    digitalWrite(STATUS_LED, LOW);
    delay(150);
  }

  // BLE 초기화
  setupBLE();
  
  Serial.println("✅ 시스템 준비 완료!");
  Serial.println("🔘 버튼을 눌러 차량 해제 신호 전송!");
  Serial.println("======================================");
}

void loop() {
  // 버튼 체크
  checkButton();
  
  // 해제 이벤트 관리
  manageUnlockEvent();
  
  // 상태 LED 관리
  manageStatusLED();
  
  // 시스템 상태 출력 (5초마다)
  printSystemStatus();
  
  delay(50);
}

// BLE 설정
void setupBLE() {
  Serial.println("📡 BLE 서버 초기화 중...");
  
  // BLE 디바이스 초기화
  BLEDevice::init("ESP32-SmartKey");
  
  // BLE 서버 생성
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // BLE 서비스 생성
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // BLE 특성 생성 (읽기 + 알림)
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());

  // 서비스 시작
  pService->start();

  // 광고 시작
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
  
  Serial.println("✅ BLE 서버 시작 완료!");
  Serial.println("📱 스캐너 연결 대기 중...");
}

// 버튼 확인
void checkButton() {
  int reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;
      
      if (currentButtonState == LOW && !unlockEventActive) {
        Serial.println("🔘 버튼 감지! 해제 신호 전송!");
        sendUnlockEvent();
      }
    }
  }
  
  lastButtonState = reading;
}

// 해제 이벤트 전송 (BLE 통신)
void sendUnlockEvent() {
  if (!deviceConnected) {
    Serial.println("⚠️ 클라이언트가 연결되지 않음");
    return;
  }
  
  Serial.println("📡 BLE로 해제 신호 전송!");
  
  // "UNLOCK" 신호 전송
  String unlockMessage = "UNLOCK";
  pCharacteristic->setValue(unlockMessage.c_str());
  pCharacteristic->notify();
  
  // 로컬 피드백
  executeLocalUnlockEffect();
  
  unlockEventActive = true;
  unlockEventStartTime = millis();
  
  Serial.println("✅ 해제 신호 전송 완료!");
}

// 로컬 해제 효과
void executeLocalUnlockEffect() {
  // 부저 소리
  playUnlockSound();
  
  // LED 효과
  unlockLightEffect();
}

// 해제 이벤트 관리
void manageUnlockEvent() {
  if (unlockEventActive && (millis() - unlockEventStartTime > UNLOCK_EVENT_DURATION)) {
    unlockEventActive = false;
    Serial.println("🔄 해제 이벤트 종료");
  }
}

// 상태 LED 관리
void manageStatusLED() {
  static unsigned long lastBlink = 0;
  unsigned long currentTime = millis();
  
  if (!deviceConnected) {
    // 연결되지 않음: 느린 깜빡임
    if (currentTime - lastBlink > 1000) {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      lastBlink = currentTime;
    }
  } else if (unlockEventActive) {
    // 해제 이벤트: 빠른 깜빡임
    if (currentTime - lastBlink > 100) {
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      lastBlink = currentTime;
    }
  } else {
    // 연결됨: 항상 켜짐
    digitalWrite(STATUS_LED, HIGH);
  }
}

// 부저 소리
void playUnlockSound() {
  // 첫 번째 톤
  for (int i = 0; i < 100; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(500);
  }
  
  delay(100);
  
  // 두 번째 톤 (더 높은 음)
  for (int i = 0; i < 120; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(400);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(400);
  }
  
  digitalWrite(BUZZER_PIN, LOW);
}

// LED 효과
void unlockLightEffect() {
  // 빠른 깜빡임
  for (int i = 0; i < 5; i++) {
    digitalWrite(UNLOCK_LED_PIN, HIGH);
    delay(100);
    digitalWrite(UNLOCK_LED_PIN, LOW);
    delay(100);
  }
  
  // 긴 점등
  digitalWrite(UNLOCK_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(UNLOCK_LED_PIN, LOW);
}

// 시스템 상태 출력
void printSystemStatus() {
  static unsigned long lastStatus = 0;
  if (millis() - lastStatus > 5000) {
    lastStatus = millis();
    
    Serial.println("📊 시스템 상태:");
    Serial.printf("   디바이스명: ESP32-SmartKey\n");
    Serial.printf("   연결 상태: %s\n", deviceConnected ? "연결됨 ✅" : "대기 중 ⏳");
    Serial.printf("   해제 이벤트: %s\n", unlockEventActive ? "활성화" : "대기");
    Serial.printf("   메모리: %d bytes\n", ESP.getFreeHeap());
    
    String macAddress = BLEDevice::getAddress().toString().c_str();
    Serial.printf("   📍 MAC 주소: %s\n", macAddress.c_str());
    
    Serial.println("   🔘 버튼을 눌러 해제 신호 전송!");
    Serial.println("----------------------------------------");
  }
}