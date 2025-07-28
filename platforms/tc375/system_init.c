#include "system_init.h"
#include "BSW/Driver/asclin.h"
#include "BSW/IO/Bluetooth.h"
#include "BSW/IO/Motor.h"
#include "can.h"
#include "canfd.h"

void Module_Init (void)
{
    Gpt1_Init();
    Motor_Init();
    Asclin1_InitUart();
    Asclin0_InitUart(); // for debug

    Can_Init(BD_500K, CAN_NODE0);
    CanFd_Init(BD_500K, HS_BD_2M, CANFD_NODE2);
}

IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;

void System_Init (void)
{
    IfxCpu_enableInterrupts();

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    Module_Init();
}
