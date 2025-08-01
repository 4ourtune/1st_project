#include "system_init.h"

void Module_Init (void)
{
    Gpt1_Init();
    scueru_Init0();
    scueru_Init1();
    scueru_Init2();
    Ultrasonic_Trigger_Init();
    Can_Init(BD_500K, CAN_NODE0);
    Motor_Init();
    Asclin1_InitUart();
    Asclin0_InitUart(); // for debug
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
