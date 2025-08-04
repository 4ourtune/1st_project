#ifndef BSW_DRIVER_ERU_H_
#define BSW_DRIVER_ERU_H_

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#include "isr_priority.h"
#include "Ultrasonic.h"

void ScuEru_Init0 (void);
void ScuEru_Init1 (void);
void ScuEru_Init2 (void);

#endif /* BSW_DRIVER_ERU_H_ */
