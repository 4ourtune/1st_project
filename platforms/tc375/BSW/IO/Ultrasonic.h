#ifndef BSW_IO_ULTRASONIC_H_
#define BSW_IO_ULTRASONIC_H_

#include "IfxCpu.h"

#include "stm.h"
#include "avg_filter.h"

void Ultrasonic_Init (void);
void Ultrasonic_Toggle_Trigger (void);
void Ultrasonic_Left_Handler (int input_pin_state);
void Ultrasonic_Right_Handler (int input_pin_state);
void Ultrasonic_Rear_Handler (int input_pin_state);
void Ultrasonic_Get_Left_Data (unsigned int *dist_mm, uint64 *timestamp_us);
void Ultrasonic_Get_Right_Data (unsigned int *dist_mm, uint64 *timestamp_us);
void Ultrasonic_Get_Rear_Data (unsigned int *dist_mm, uint64 *timestamp_us);

#endif /* BSW_IO_ULTRASONIC_H_ */
