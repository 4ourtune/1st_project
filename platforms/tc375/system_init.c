#include "system_init.h"
#include "BSW/Driver/asclin.h"
#include "BSW/IO/Bluetooth.h"
#include "BSW/IO/Motor.h"

void System_Init(void)
{
    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Initialize BSW */
    Asclin0_InitUart();    /* Bluetooth UART */
    Asclin1_InitUart();    /* Debug/Additional UART */
    Bluetooth_Init();      /* Bluetooth module */
    Motor_Init();          /* Motor hardware initialization */
}
