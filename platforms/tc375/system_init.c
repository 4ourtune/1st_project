#include "system_init.h"

void Module_Init (void)
{
    Asclin0_InitUart(); // For usb in/out
    Asclin1_InitUart(); // For bluetooth
    Asclin2_InitUart(); // For ToF
    ScuEru_Init0(); // For ultrasonic
    ScuEru_Init1(); // For ultrasonic
    ScuEru_Init2(); // For ultrasonic
    GPIO_Init(); // For buzzer, led, ultrasonic
    Gpt2_Init(); // For buzzer
    Stm0_Init(); // For ultrasonic

    Bluetooth_Init();
    Buzzer_Init();
    LED_Init();
    Motor_Init();
    ToF_Init();
    Ultrasonic_Init();
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
