#include "system_init.h"
#include "BSW/Driver/asclin.h"
#include "BSW/IO/Bluetooth.h"
#include "BSW/IO/Motor.h"
#include "can.h"
#include "canfd.h"

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

    Can_Init(BD_500K, CAN_NODE0);
    CanFd_Init(BD_500K, HS_BD_2M, CANFD_NODE2);
}
