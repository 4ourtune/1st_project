#include "aps.h"
#include "my_stdio.h"
#include <stdbool.h> // bool 타입 사용

#define CYCLE_DELAY_US 100000 // 100000us = 100ms
#define SENSOR_DATA_COUNT 4
#define APS_WALL_THRESHOLD_CM 30
#define MIN_PARKING_SPACE_CM 50
#define APS_REAR_SAFETY_DISTANCE_CM 5
#define APS_VEHICLE_SPEED_CM_PER_MS 0.5f
#define APS_MAX_SPACE_SIZE_CM 1000

typedef enum {
    WALL_DETECTED,
    SPACE_DETECTED
} APS_WallSpaceState_t;

typedef enum {
    PHASE_SPACE_DETECTION,
    PHASE_PARKING_EXECUTION,
    PHASE_COMPLETED
} APS_ParkingPhase_t;

static unsigned int sense_dist[SENSOR_DATA_COUNT];
static uint64 sense_time[SENSOR_DATA_COUNT];

static volatile int aps_state = 0;
static int result_x;               // APS가 판단한 조향 명령 (0~99, 50: 중립)
static int result_y;               // APS가 판단한 속도 명령 (0~99, 50: 정지)
static int is_APS_done;            // APS 주차 완료 플래그 (0: 진행중, 1: 완료)

static APS_ParkingPhase_t current_phase = PHASE_SPACE_DETECTION;
static APS_WallSpaceState_t current_state = WALL_DETECTED;
static int wall_reference_distance = 0;
static bool wall_reference_initialized = false;
static uint64 space_start_time = 0;
static uint64 space_end_time = 0;
static float measured_space_size = 0.0f;

// 공간 탐지 상태 분석
static APS_WallSpaceState_t APS_AnalyzeSpace(int distance)
{
    if (!wall_reference_initialized) {
        wall_reference_distance = distance;
        wall_reference_initialized = true;
        return WALL_DETECTED;
    }
    if (distance < wall_reference_distance) {
        wall_reference_distance = distance;
    }
    if (distance > (wall_reference_distance + APS_WALL_THRESHOLD_CM)) {
        return SPACE_DETECTED;
    } else {
        return WALL_DETECTED;
    }
}

// 공간 탐지 상태 전이 및 시간 기록
static void APS_ProcessStateTransition(APS_WallSpaceState_t new_state)
{
    if (current_state != new_state) {
        if (current_state == WALL_DETECTED && new_state == SPACE_DETECTED) {
            space_start_time = getTimeUs();
            current_state = SPACE_DETECTED;
        }
        else if (current_state == SPACE_DETECTED && new_state == WALL_DETECTED) {
            space_end_time = getTimeUs();
            measured_space_size = APS_CalculateSpaceSize(space_start_time, space_end_time);
            current_state = WALL_DETECTED;
        }
    }
}

// 공간 크기 계산
static float APS_CalculateSpaceSize(uint64 start_time, uint64 end_time)
{
    if (end_time < start_time) return 0.0f;
    uint64 time_diff_us = end_time - start_time;
    float time_diff_ms = time_diff_us / 1000.0f;
    float space_size = time_diff_ms * APS_VEHICLE_SPEED_CM_PER_MS;
    if (space_size > APS_MAX_SPACE_SIZE_CM) return 0.0f;
    return space_size;
}

// 공간 탐지 및 주차 가능 판단
static bool APS_DetectParkingSpace(void)
{
    int left_distance = sense_dist[1];
    APS_WallSpaceState_t analyzed_state = APS_AnalyzeSpace(left_distance);
    APS_ProcessStateTransition(analyzed_state);

    if (measured_space_size >= MIN_PARKING_SPACE_CM) {
        return true;
    }
    return false;
}

/**
 * @brief 센서 데이터를 갱신하고, APS 명령을 계산하는 함수
 */
int Update_APS_Result (uint64 interval_us)
{
    unsigned short tof_signal;
    unsigned char tof_status;
    Tof_GetDistance(sense_dist + 0, &tof_signal, &tof_status, sense_time + 0); // ToF_front
    Ultrasonic_Get_Left_Data(sense_dist + 1, sense_time + 1); // Ult_left
    Ultrasonic_Get_Right_Data(sense_dist + 2, sense_time + 2); // Ult_right
    Ultrasonic_Get_Rear_Data(sense_dist + 3, sense_time + 3); // Ult_rear

    uint64 cur_time = getTimeUs();

    for (int i = 0; i < SENSOR_DATA_COUNT; i++)
    {
        uint64 sensor_delay = cur_time - sense_time[i];
        if (sensor_delay > interval_us)
            return 0;
    }

    Calc_APS_Result();
    return 1;
}

/**
 * @brief APS가 판단한 조향/속도 명령을 계산하는 함수
 */
void Calc_APS_Result (void)
{
    int left_distance = sense_dist[1];
    int rear_distance = sense_dist[3];

    switch (current_phase) {
        case PHASE_SPACE_DETECTION:
            result_x = 50; // 중립(직진)
            result_y = 70; // 저속 전진
            if (APS_DetectParkingSpace()) {
                current_phase = PHASE_PARKING_EXECUTION;
            }
            return;
        case PHASE_PARKING_EXECUTION:
            if (rear_distance <= APS_REAR_SAFETY_DISTANCE_CM) {
                result_x = 50;
                result_y = 50; // 정지
                current_phase = PHASE_COMPLETED;
                return;
            }
            result_x = APS_CalculateSteeringFromSensor(left_distance);
            result_y = 30; // 후진
            return;
        case PHASE_COMPLETED:
            aps_state = 0;
            is_APS_done = 1;
            result_x = 50;
            result_y = 50; // 정지
            return;
        default:
            result_x = 50;
            result_y = 50;
            return;
    }
}

/**
 * @brief 센서 거리값을 기반으로 조향 명령(x값)을 계산하는 함수
 */
int APS_CalculateSteeringFromSensor(int sensor_distance)
{
    int base_steering = 30;
    if (sensor_distance < 15) {
        base_steering = 70;
    } else if (sensor_distance > 40) {
        base_steering = 20;
    }
    if (base_steering < 0) base_steering = 0;
    if (base_steering > 99) base_steering = 99;
    return base_steering;
}

/**
 * @brief APS가 판단한 결과(조향/속도 명령, 주차 완료 여부)를 외부로 반환하는 함수
 */
void Get_APS_Result (int *res_x, int *res_y, int *is_done)
{
    *res_x = result_x;
    *res_y = result_y;
    *is_done = is_APS_done;
}

/**
 * @brief APS 상태를 설정하는 함수
 */
void Set_APS_State (int state)
{
    if (state < 0 || state > 1) {
        my_printf("[APS] Invalid state value: %d\n", state);
        return;
    }
    aps_state = state;
}
