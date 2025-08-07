#ifndef BSW_DRIVER_STM_H_
#define BSW_DRIVER_STM_H_

#include "Ifx_Types.h"
#include "IfxStm.h"

#include "isr_priority.h"
#include "GPIO.h"

uint64 getTime10ns (void);
uint64 getTimeUs (void);
uint64 getTimeMs (void);

void Stm0_Init (void);
void Stm0_InterruptAfter (uint64 delay_us);

#endif /* BSW_DRIVER_STM_H_ */
