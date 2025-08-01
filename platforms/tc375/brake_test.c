#include "brake_test.h"
#include "BSW/etc/my_stdio.h"


// 테스트 상태 관리
static bool is_test_running = false;
static int current_motor_speed = 0;
static uint32_t test_start_time = 0;
static float initial_distance_mm = 0.0f;

#define TARGET_DISTANCE_MM 500.0f  // 50cm = 500mm
#define DISTANCE_CHECK_KEY 't'

// 외부 TOF 함수
extern unsigned int Tof_GetCorrectedDistance(void);

// 0~9 입력에 따른 테스트 시작
void BrakeTest_ProcessInput(char input) {
    if (input >= '0' && input <= '9') {
        BrakeTest_StartSpeedTest(input);
    } else if (input == DISTANCE_CHECK_KEY || input == 'T') {
        BrakeTest_CheckCurrentDistance();
    } else {
        my_printf("[BrakeTest] Invalid input: %c (Use 0-9 for speed, 't' for distance check)\n", input);
    }
}

// 속도별 테스트 시작
void BrakeTest_StartSpeedTest(char speed_input) {
    if (is_test_running) {
        my_printf("[BrakeTest] Test already running! Press 't' to check distance.\n");
        return;
    }
    
    // 초기 거리 측정
    initial_distance_mm = (float)Tof_GetCorrectedDistance();
    
    if (initial_distance_mm < TARGET_DISTANCE_MM) {
        my_printf("[BrakeTest] Error: Initial distance %.1f mm is less than target %.1f mm\n", 
                  initial_distance_mm, TARGET_DISTANCE_MM);
        my_printf("[BrakeTest] Move vehicle further from obstacle and try again.\n");
        return;
    }
    
    // 입력에 따른 모터 속도 설정
    int motor_speeds[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int speed_index = speed_input - '0';
    current_motor_speed = motor_speeds[speed_index];
    
    // 모터 시작
    MotorController_SetSpeed(current_motor_speed, current_motor_speed);
    
    my_printf("=== Brake Test Started ===\n");
    my_printf("Input: %c -> Motor Speed: %d%%\n", speed_input, current_motor_speed);
    my_printf("Initial Distance: %.1f mm\n", initial_distance_mm);
    my_printf("Target Distance: %.1f mm (Auto brake)\n", TARGET_DISTANCE_MM);
    my_printf("Press 't' to check current distance\n");
    my_printf("========================\n");
}

// 현재 거리 확인 ('t' 입력 시)
void BrakeTest_CheckCurrentDistance(void) {   
    unsigned int current_distance_mm = Tof_GetCorrectedDistance();
    
    my_printf("end brake Distance: %u mm\n", current_distance_mm);
    my_printf("====================\n");
}

// 50cm 도달 체크 및 자동 제동
bool BrakeTest_CheckAndBrake(void) {
    if (!is_test_running) return false;
    
    float current_distance_mm = (float)Tof_GetCorrectedDistance();
    
    // 50cm(500mm) 도달 시 자동 제동
    if (current_distance_mm <= TARGET_DISTANCE_MM) {
        float traveled_distance_mm = initial_distance_mm - current_distance_mm;
        float actual_brake_distance_mm = current_distance_mm;
        
        // 모터 정지
        MotorController_SetSpeed(0, 0);
        
        my_printf("=== AUTO BRAKE APPLIED ===\n");
        my_printf("Motor Speed: %d%%\n", current_motor_speed);
        my_printf("Initial Distance: %.1f mm\n", initial_distance_mm);
        my_printf("Final Distance: %.1f mm\n", current_distance_mm);
        my_printf("Traveled Distance: %.1f mm\n", traveled_distance_mm);
        my_printf("Target Distance: %.1f mm\n", TARGET_DISTANCE_MM);
        my_printf("Brake Distance: %.1f mm (%.2f cm)\n", actual_brake_distance_mm, actual_brake_distance_mm/10.0f);
        my_printf("===========================\n");
        
        // 테스트 종료
        is_test_running = false;
        current_motor_speed = 0;
        return true;
    }
    
    return false;
}

// 테스트 상태 모니터링 (메인 루프에서 호출)
void BrakeTest_Update(void) {
    if (!is_test_running) return;
    
    // 50cm 도달 체크 및 자동 제동
    if (BrakeTest_CheckAndBrake()) {
        return; // 제동 완료
    }
}

// 현재 테스트 상태 확인
bool BrakeTest_IsRunning(void) {
    return is_test_running;
}

// 테스트 강제 종료
void BrakeTest_Stop(void) {
    if (is_test_running) {
        MotorController_SetSpeed(0, 0);
        is_test_running = false;
        current_motor_speed = 0;
        my_printf("[BrakeTest] Test stopped manually.\n");
    }
}

// 현재 TOF 센서 값 확인
void BrakeTest_ShowTofStatus(void) {
    float current_distance_mm = (float)Tof_GetCorrectedDistance();
    my_printf("=== TOF Sensor Status ===\n");
    my_printf("Current Distance: %.1f mm (%.2f cm)\n", current_distance_mm, current_distance_mm/10.0f);
    my_printf("Target Distance: %.1f mm (%.2f cm)\n", TARGET_DISTANCE_MM, TARGET_DISTANCE_MM/10.0f);
    
    if (current_distance_mm > TARGET_DISTANCE_MM) {
        my_printf("Status: Ready for test\n");
    } else {
        my_printf("Status: Too close - move back %.1f mm\n", TARGET_DISTANCE_MM - current_distance_mm);
    }
    my_printf("========================\n");
}

// 테스트 정보 출력
void BrakeTest_ShowInfo(void) {
    my_printf("=== Brake Test Info ===\n");
    my_printf("Commands:\n");
    my_printf("  0: 10%% speed test\n");
    my_printf("  1: 20%% speed test\n");
    my_printf("  2: 30%% speed test\n");
    my_printf("  3: 40%% speed test\n");
    my_printf("  4: 50%% speed test\n");
    my_printf("  5: 60%% speed test\n");
    my_printf("  6: 70%% speed test\n");
    my_printf("  7: 80%% speed test\n");
    my_printf("  8: 90%% speed test\n");
    my_printf("  9: 100%% speed test\n");
    my_printf("  t: Check current distance\n");
    my_printf("  s: Show TOF sensor status\n");
    my_printf("Auto brake at: %.1f mm (%.1f cm)\n", TARGET_DISTANCE_MM, TARGET_DISTANCE_MM/10.0f);
    my_printf("Status: %s\n", is_test_running ? "RUNNING" : "IDLE");
    
    if (is_test_running) {
        my_printf("Current Speed: %d%%\n", current_motor_speed);
        float current_dist = (float)Tof_GetCorrectedDistance();
        my_printf("Current Distance: %.1f mm\n", current_dist);
    }
    my_printf("=====================\n");
}

// 's' 입력 추가 처리
void BrakeTest_ProcessInputExtended(char input) {
    if (input >= '0' && input <= '9') {
        BrakeTest_StartSpeedTest(input);
    } else if (input == DISTANCE_CHECK_KEY || input == 'T') {
        BrakeTest_CheckCurrentDistance();
    } else if (input == 's' || input == 'S') {
        BrakeTest_ShowTofStatus();
    } else if (input == 'h' || input == 'H') {
        BrakeTest_ShowInfo();
    } else {
        my_printf("[BrakeTest] Invalid input: %c\n", input);
        my_printf("Use: 0-9 (speed), 't' (distance), 's' (sensor), 'h' (help)\n");
    }
}