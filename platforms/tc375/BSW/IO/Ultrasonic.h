#ifndef BSW_IO_ULTRASONIC_H_
#define BSW_IO_ULTRASONIC_H_

#include "stm.h"
#include "avg_filter.h"

#include <stdbool.h>

typedef enum
{
    ULTRASONIC_LEFT = 0, ULTRASONIC_RIGHT, ULTRASONIC_REAR, ULTRASONIC_COUNT
} UltrasonicSide;

typedef struct
{
    unsigned int distance_mm;
    uint64_t timestamp_us;
} UltrasonicData_t;

void Ultrasonic_Init (void);
void Ultrasonic_Toggle_Trigger (void);

void Ultrasonic_EchoHandler (UltrasonicSide side, int input_pin_state);
bool Ultrasonic_GetLatestData (UltrasonicSide side, UltrasonicData_t *out);

#endif /* BSW_IO_ULTRASONIC_H_ */
