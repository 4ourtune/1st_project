#ifndef BSW_DRIVER_ERU_H_
#define BSW_DRIVER_ERU_H_

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#include "isr_priority.h"
#include "stm.h"
#include "avg_filter.h"

void scueru_Init0 (void);
void scueru_Init1 (void);
void scueru_Init2 (void);
void get_eru_int0_data (unsigned int *dist_mm, uint64 *timestamp_us); // left
void get_eru_int1_data (unsigned int *dist_mm, uint64 *timestamp_us); // right
void get_eru_int2_data (unsigned int *dist_mm, uint64 *timestamp_us); // rear

#endif /* BSW_DRIVER_ERU_H_ */
