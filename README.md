# 차량 임베디드 SW 프로젝트

## 프로젝트 개요

이 프로젝트는 여러 하드웨어 플랫폼을 통합한 고급 임베디드 시스템을 보여주는 다중 플랫폼 자동차 웰컴라이트 시스템입니다. 여러 마이크로컨트롤러 플랫폼이 협력하여 무선 통신을 통한 차량 제어 시스템을 구현합니다.

### 주요 기능

- **다중 플랫폼 아키텍처**: TC375 (Infineon TriCore), ESP32 (WiFi/Bluetooth), Raspberry Pi 4
- **무선 통신**: BLE, WiFi 및 커스텀 프로토콜
- **안전 시스템**: 자동 긴급 제동 시스템 (AEB), 적응형 위치 시스템 (APS)
- **실시간 제어**: 하드웨어 추상화를 통한 멀티코어 처리
- **동적 테스트**: Google Test 프레임워크 통합
- **정적 분석**: 코드 품질을 위한 Cppcheck 통합

## 아키텍처

```
┌─────────────────────────────────────────────────────────────────┐
│                   원격제어 및 웰컴라이트 시스템                   │
├─────────────────┬─────────────────┬─────────────────────────────┤
│  Raspberry Pi 4 │      ESP32      │     TC375 (TriCore)         │
│   (컨트롤러)     │   (BLE 비콘)    │      (메인 ECU)             │
│                 │                 │                             │
│   • Python      │   • Arduino     │   • 멀티코어 C/C++          │
│   • Bluetooth   │   • BLE Server  │   • Infineon iLLD           │
│   • 메인 로직    │  • 웰컴라이트   │   • 안전 시스템              │
│                 │                 │   • 모터 제어                │
└─────────────────┴─────────────────┴─────────────────────────────┘
```

## 디렉토리 구조

### 핵심 플랫폼

```
platforms/
├── tc375/                    # Infineon TriCore TC375 마이크로컨트롤러
│   ├── ASW/                  # 응용 소프트웨어 계층
│   │   ├── AEB/              # 자동 긴급 제동 시스템
│   │   └── APS/              # 적응형 위치 시스템
│   ├── BSW/                  # 기본 소프트웨어 계층
│   │   ├── Controller/       # 상위 레벨 컨트롤러
│   │   ├── Driver/           # 하드웨어 드라이버
│   │   └── IO/               # 입출력 모듈
│   ├── Libraries/            # Infineon iLLD 및 서비스 계층
│   ├── Configurations/       # 하드웨어 및 소프트웨어 구성
│   ├── Utils/                # 유틸리티 함수 및 데이터 구조
│   └── Cpu[0-2]_Main.c      # 멀티코어 메인 애플리케이션
├── esp32/                    # ESP32 모듈
│   ├── esp32_beacon/         # BLE 웰컴라이트 비콘
│   └── esp32-scanner/        # BLE 스캐너 모듈
└── Raspberry_Pi_4/           # Raspberry Pi 컨트롤러
    ├── main.py              # 메인 애플리케이션 진입점
    ├── controller.py        # 핵심 컨트롤러 로직
    ├── bluetooth.py         # Bluetooth 통신
    └── joystick.py          # 입력 처리
```

### 개발 도구

```
tools/
├── cppcheck/                 # 정적 분석 도구 (전체 배포)
├── googletest/               # 단위 테스트 프레임워크
├── cppcheck_config/          # Cppcheck 구성 파일
└── git_hooks/                # Git 자동화 스크립트
```

### 테스트 인프라

```
test/
├── CMakeLists.txt           # Google Test용 CMake 구성
├── example_test.cpp         # 예시 단위 테스트
├── test_single.ps1          # PowerShell 테스트 실행기
└── build/                   # 빌드 출력 디렉토리
```

## 기술적 세부사항

### TC375 TriCore 플랫폼

**주요 구성요소:**
- **AEB 시스템**: ToF/초음파 센서를 이용한 자동 긴급 제동
- **모터 컨트롤러**: 피드백을 통한 PWM 기반 모터 제어
- **통신 스택**: CAN, SPI, UART 인터페이스
- **안전 프레임워크**: 워치독, 오류 처리, 결함 감지

**하드웨어 추상화:**
- Infineon iLLD (infineon Low Level Drivers)
- 표준화된 인터페이스 (IfxStdIf)
- 플랫폼별 구성

### ESP32 BLE 비콘

**기능:**
- BLE 서버 구현
- 웰컴라이트 인증
- 오디오/비주얼 피드백 시스템
- 저전력 동작 모드

### Raspberry Pi 컨트롤러

**기능:**
- 중앙 제어 로직
- Bluetooth 통신 관리
- 사용자 인터페이스 처리
- 시스템 오케스트레이션

## 개발 도구 통합

### 지원 IDE
- **TASKING VX**: 주요 TC375 개발
- **VS Code**: 다중 플랫폼 편집
- **Arduino IDE**: ESP32 개발
- **Visual Studio**: Windows 테스트

### 빌드 시스템
- **TASKING**: TC375 임베디드 빌드
- **CMake**: 크로스 플랫폼 테스트
- **Arduino**: ESP32 컴파일
- **Make**: 대안 TC375 빌드

### 품질 보증
- **Cppcheck**: 정적 분석 (포함됨)
- **Google Test**: 단위 테스트 프레임워크
- **Git Hooks**: 자동화된 품질 검사
- **MISRA-C**: 코딩 표준 준수

## 의존성

### TC375 플랫폼
- Infineon iLLD TC37A
- TASKING VX-toolset
- 표준 C 라이브러리

### ESP32 플랫폼
- Arduino ESP32 Core
- BLE 라이브러리
- WiFi 스택

### Raspberry Pi
- Python 3.8+
- Bluetooth 라이브러리
- GPIO 접근 라이브러리

### 테스트 프레임워크
- Google Test 1.14.0
- CMake 3.16+
- MSVC 2019+ 또는 GCC