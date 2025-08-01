#ifndef BRAKE_TEST_H
#define BRAKE_TEST_H

#include <stdbool.h>
#include <stdint.h>
#include "motor_controller.h"
#include "ToF.h"

// 상수 정의
#define TARGET_DISTANCE_MM 500.0f  // 50cm = 500mm
#define DISTANCE_CHECK_KEY 't'

// 외부 함수 선언 (다른 모듈에서 구현)
extern void MotorController_SetSpeed(int speedA, int speedB);
extern unsigned int Tof_GetCorrectedDistance(void);

// 메인 인터페이스 함수들
void BrakeTest_ProcessInput(char input);
void BrakeTest_ProcessInputExtended(char input);
void BrakeTest_Update(void);

// 테스트 제어 함수들
void BrakeTest_StartSpeedTest(char speed_input);
void BrakeTest_CheckCurrentDistance(void);
bool BrakeTest_CheckAndBrake(void);
void BrakeTest_Stop(void);

// 상태 확인 함수들
bool BrakeTest_IsRunning(void);
void BrakeTest_ShowTofStatus(void);
void BrakeTest_ShowInfo(void);

#endif // BRAKE_TEST_H