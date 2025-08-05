#ifndef SYSTEM_INIT_H_
#define SYSTEM_INIT_H_

#include <emer_light.h>
#include <GPIO.h>
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#include "Motor.h"
#include "eru.h"
#include "Ultrasonic.h"
#include "gpt12.h"
#include "can.h"
#include "asclin.h"
#include "stm.h"

void System_Init (void);

#endif /* SYSTEM_INIT_H_ */
