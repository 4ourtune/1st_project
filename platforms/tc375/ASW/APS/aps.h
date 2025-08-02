#ifndef ASW_APS_APS_H_
#define ASW_APS_APS_H_

/**
#include <stdbool.h>
#include <stdint.h>

#define APS_VEHICLE_SPEED_CM_PER_MS     0.5f    // Vehicle speed constant (needs real measurement calibration)
#define MIN_PARKING_SPACE_CM            50      // Minimum parking space required
#define APS_WALL_THRESHOLD_CM           30      // Wall detection threshold
#define APS_REAR_SAFETY_DISTANCE_CM     5       // Rear safety distance for parking
#define APS_PARKING_TIMEOUT_SEC         30      // Parking operation timeout
#define APS_MAX_SPACE_SIZE_CM           1000    // Maximum realistic space size filter

typedef enum {
    WALL_DETECTED,
    SPACE_DETECTED
} APS_WallSpaceState_t;

typedef enum {
    PHASE_SPACE_DETECTION,     // Space detection phase
    PHASE_PARKING_EXECUTION,   // Parking execution phase
    PHASE_COMPLETED            // Parking completed phase
} APS_ParkingPhase_t;

typedef struct {
    int x;  // 좌우 방향 (0~99)
    int y;  // 전후 방향 (0~99)
} APS_Coordinate_t;

// State management functions
void APS_ToggleAutoParkingState(void);
bool APS_IsAutoParkingActive(void);

// Core coordinate-based parking functions
APS_Coordinate_t APS_GetParkingCommand(void);
APS_Coordinate_t APS_GetSpaceDetectionCommand(void);
APS_Coordinate_t APS_GetParkingManeuverCommand(void);

// Space detection and analysis
APS_WallSpaceState_t APS_AnalyzeSpace(int distance);
void APS_ProcessStateTransition(APS_WallSpaceState_t new_state);
float APS_CalculateSpaceSize(uint32_t start_time, uint32_t end_time);
bool APS_DetectParkingSpace(void);

// Sensor-based coordinate calculation
int APS_CalculateSteeringFromSensor(int sensor_distance);

// Utility functions
float APS_GetLastMeasuredSpaceSize(void);
*/
#include "ToF.h"
#include "eru.h"
#include "stm.h"

int Get_APS_State (void);
void Set_APS_State (int state);
int Get_APS_Result (int *res_x, int *res_y);

#endif /* ASW_APS_APS_H_ */
