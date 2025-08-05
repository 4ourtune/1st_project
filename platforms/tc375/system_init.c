#include "system_init.h"

void Module_Init (void)
{
    GPIO_Init();
    Motor_Init();
    ScuEru_Init0();
    ScuEru_Init1();
    ScuEru_Init2();
    Ultrasonic_Init();
    Gpt1_Init();
    Asclin0_InitUart(); // for debug
    Asclin1_InitUart();
    Asclin2_InitUart(); // For ToF
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
