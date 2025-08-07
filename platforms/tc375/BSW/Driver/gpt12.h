#ifndef BSW_DRIVER_GPT12_H_
#define BSW_DRIVER_GPT12_H_

#include <Ifx_reg.h>
#include <Ifx_Types.h>
#include <IfxGpt12.h>
#include <IfxPort.h>

#include "isr_priority.h"
#include "GPIO.h"
#include "stm.h"

#include "Buzzer.h"

#define ULTRASONIC_TRIG_HIGH_DURATION 1000 // 1000us = 1ms

void Gpt1_Init (void);
void Run_Gpt12_T3 (void);
void Stop_Gpt12_T3 (void);

void Gpt2_Init (void);
void Run_Gpt12_T6 (void);
void Stop_Gpt12_T6 (void);

#endif /* BSW_DRIVER_GPT12_H_ */
