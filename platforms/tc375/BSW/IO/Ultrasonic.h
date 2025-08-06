#ifndef BSW_IO_ULTRASONIC_H_
#define BSW_IO_ULTRASONIC_H_

#include <stdint.h>
#include <stdbool.h>

#include "ult_event_queue.h"
#include "avg_filter.h"
#include "stm.h"

#define ULT_FILTER_WINDOW_SIZE 5
#define CALCULATE_DISTANCE(fall_time, rise_time) (((int32_t)((fall_time) - (rise_time)) * 343) / 2000) // Speed of sound : 0.343 mm/us

typedef enum
{
    ULTRASONIC_LEFT = 0, ULTRASONIC_RIGHT, ULTRASONIC_REAR, ULTRASONIC_COUNT
} UltrasonicSide;

typedef struct
{
    int distance_mm;
    uint64_t received_time_us;
} UltrasonicData_t;

void Ultrasonic_Init (void);
void Ultrasonic_ToggleTrigger (void);
void Ultrasonic_EchoHandler (UltrasonicSide side, int input_pin_state);
void Ultrasonic_ProcessQueue (void);
bool Ultrasonic_GetLatestData (UltrasonicSide side, UltrasonicData_t *out);

#endif /* BSW_IO_ULTRASONIC_H_ */
