#include "aps.h"
#include "BSW/etc/my_stdio.h"

static volatile bool auto_parking_state = false;
static uint32_t space_start_time = 0;
static uint32_t space_end_time = 0;
static APS_WallSpaceState_t current_state = WALL_DETECTED;
static float measured_space_size = 0.0f;
static int wall_reference_distance = 0;
static bool wall_reference_initialized = false;
static APS_ParkingPhase_t current_phase = PHASE_SPACE_DETECTION;
static uint32_t operation_start_time = 0;

// External hardware interface functions (to be implemented in BSW/IO/Ultrasonic)
extern int UltrasonicSensor_GetDistance_Left(void);
extern int UltrasonicSensor_GetDistance_Right(void);
extern int UltrasonicSensor_GetDistance_Rear(void);
extern void MotorController_SetSpeed(int speedA, int speedB);
extern uint32_t GetCurrentTime_ms(void); // System timer function

void APS_ToggleAutoParkingState(void)
{
    auto_parking_state = !auto_parking_state;
    my_printf("[APS] Auto parking state toggled: %s\n", auto_parking_state ? "ACTIVE" : "INACTIVE");
    
    if (!auto_parking_state) {
        my_printf("[APS] Auto parking deactivated\n");
    } else {
        // Reset to initial phase when activated
        current_phase = PHASE_SPACE_DETECTION;
        operation_start_time = GetCurrentTime_ms();
        wall_reference_initialized = false;
        measured_space_size = 0.0f;
        current_state = WALL_DETECTED;
        my_printf("[APS] Auto parking activated - starting space detection phase\n");
    }
}

bool APS_IsAutoParkingActive(void)
{
    return auto_parking_state;
}

APS_WallSpaceState_t APS_AnalyzeSpace(int distance)
{
    if (!wall_reference_initialized) {
        wall_reference_distance = distance;
        wall_reference_initialized = true;
        my_printf("[APS] Wall reference distance initialized: %d cm\n", wall_reference_distance);
        return WALL_DETECTED;
    }
    
    if (distance < wall_reference_distance) {
        wall_reference_distance = distance;
        my_printf("[APS] Wall reference updated: %d cm\n", wall_reference_distance);
    }
    
    if (distance > (wall_reference_distance + APS_WALL_THRESHOLD_CM)) {
        return SPACE_DETECTED;
    } else {
        return WALL_DETECTED;
    }
}

void APS_ProcessStateTransition(APS_WallSpaceState_t new_state)
{
    if (current_state != new_state) {
        if (current_state == WALL_DETECTED && new_state == SPACE_DETECTED) {
            space_start_time = GetCurrentTime_ms();
            current_state = SPACE_DETECTED;
            my_printf("[APS] State: WALL→SPACE, Time: %lu ms\n", space_start_time);
            my_printf("[APS] Space detection started\n");
        }
        else if (current_state == SPACE_DETECTED && new_state == WALL_DETECTED) {
            space_end_time = GetCurrentTime_ms();
            measured_space_size = APS_CalculateSpaceSize(space_start_time, space_end_time);
            current_state = WALL_DETECTED;
            my_printf("[APS] State: SPACE→WALL, Time: %lu ms\n", space_end_time);
            my_printf("[APS] Space detected: %.1f cm\n", measured_space_size);
        }
    }
}

float APS_CalculateSpaceSize(uint32_t start_time, uint32_t end_time)
{
    if (end_time < start_time) {
        my_printf("[APS] Error: Invalid timestamp - end_time < start_time\n");
        return 0.0f;
    }
    
    uint32_t time_diff = end_time - start_time;
    float space_size = time_diff * APS_VEHICLE_SPEED_CM_PER_MS;
    
    if (space_size > APS_MAX_SPACE_SIZE_CM) {
        my_printf("[APS] Warning: Unrealistic space size %.1f cm filtered\n", space_size);
        return 0.0f;
    }
    
    return space_size;
}

bool APS_DetectParkingSpace(void)
{
    if (!APS_IsAutoParkingActive()) {
        return false;
    }
    
    int left_distance = UltrasonicSensor_GetDistance_Left();
    APS_WallSpaceState_t analyzed_state = APS_AnalyzeSpace(left_distance);
    APS_ProcessStateTransition(analyzed_state);
    
    if (measured_space_size >= MIN_PARKING_SPACE_CM) {
        my_printf("[APS] Space size: %.1f cm (Required: %d cm) - PARKING POSSIBLE\n", 
               measured_space_size, MIN_PARKING_SPACE_CM);
        return true;
    }
    
    return false;
}

float APS_GetLastMeasuredSpaceSize(void)
{
    return measured_space_size;
}

APS_Coordinate_t APS_GetParkingCommand(void)
{
    if (!APS_IsAutoParkingActive()) {
        return (APS_Coordinate_t){50, 50}; // 정지
    }
    
    switch (current_phase) {
        case PHASE_SPACE_DETECTION:
            return APS_GetSpaceDetectionCommand();
        case PHASE_PARKING_EXECUTION:
            return APS_GetParkingManeuverCommand();
        case PHASE_COMPLETED:
            APS_ToggleAutoParkingState(); // 자동주차 종료
            return (APS_Coordinate_t){50, 50};
        default:
            return (APS_Coordinate_t){50, 50};
    }
}

APS_Coordinate_t APS_GetSpaceDetectionCommand(void)
{
    // 공간 탐지 중: {50, 70} 반환 (직진 저속 전진)
    APS_Coordinate_t command = {50, 70};
    
    // 한 사이클 공간 탐지 수행
    if (APS_DetectParkingSpace()) {
        // 충분한 공간 발견 시 다음 단계로 전환
        current_phase = PHASE_PARKING_EXECUTION;
        my_printf("[APS] Phase changed: DETECTION → PARKING\n");
    }
    
    my_printf("[APS] Command: x=%d, y=%d\n", command.x, command.y);
    return command;
}

APS_Coordinate_t APS_GetParkingManeuverCommand(void)
{
    int rear_distance = UltrasonicSensor_GetDistance_Rear();
    int left_distance = UltrasonicSensor_GetDistance_Left();
    
    // 후방 안전 거리 체크
    if (rear_distance <= APS_REAR_SAFETY_DISTANCE_CM) {
        my_printf("[APS] Parking completed - rear safety distance reached: %d cm\n", rear_distance);
        current_phase = PHASE_COMPLETED;
        return (APS_Coordinate_t){50, 50}; // 정지
    }
    
    // 센서 기반 조향 계산
    int steering_x = APS_CalculateSteeringFromSensor(left_distance);
    int reverse_y = 20; // 후진
    
    APS_Coordinate_t command = {steering_x, reverse_y};
    my_printf("[APS] Command: x=%d, y=%d\n", command.x, command.y);
    return command;
}

int APS_CalculateSteeringFromSensor(int sensor_distance)
{
    // 기본 조향값 (좌회전하며 후진)
    int base_steering = 30;
    
    // 센서 거리에 따른 조향 보정
    if (sensor_distance < 15) {
        // 좌측 센서 가까움: 우회전으로 보정
        base_steering = 70;
        my_printf("[APS] Steering adjusted: %d (right turn - too close to left)\n", base_steering);
    } else if (sensor_distance > 40) {
        // 좌측 센서 멀음: 좌회전으로 보정  
        base_steering = 20;
        my_printf("[APS] Steering adjusted: %d (left turn - too far from left)\n", base_steering);
    } else {
        // 적절한 거리: 기본 좌회전 유지
        my_printf("[APS] Steering adjusted: %d (normal left turn)\n", base_steering);
    }
    
    // 좌표 범위 제한 (0~99)
    if (base_steering < 0) base_steering = 0;
    if (base_steering > 99) base_steering = 99;
    
    return base_steering;
}
