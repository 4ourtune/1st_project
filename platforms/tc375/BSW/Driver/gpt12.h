#ifndef BSW_DRIVER_GPT12_H_
#define BSW_DRIVER_GPT12_H_

#include <Ifx_reg.h>
#include <Ifx_Types.h>
#include <IfxGpt12.h>
#include <IfxPort.h>

#include "isr_priority.h"
#include "Ultrasonic.h"

void Gpt1_Init (void);
void Gpt2_init(void);
void runGpt12_T6(void);
void stopGpt12_T6(void);

#endif /* BSW_DRIVER_GPT12_H_ */
