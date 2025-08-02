#include "gpt12.h"
// fGPT = 100MHz = 10^8Hz = (2^8 * 5^8)Hz
static const int GPT1_BLOCK_PRESCALER = 0x2; // Set GPT1 block prescaler: 2^5 = 32
static const int TIMER_T3_INPUT_PRESCALER = 0x0; // Set T3 input prescaler: 2^0 = 0
static const int TIMER_T3_T2_VALUE = 3125; // Set timer T3, T2 value: 5^5 = 3125

IFX_INTERRUPT(IsrGpt1T3Handler, 0, ISR_PRIORITY_GPT1T3_TIMER);
void IsrGpt1T3Handler (void) // (2^3 * 5^3)Hz = 1000Hz = 0.001sec = 1ms
{
    static int aeb_cnt = 0;
    static int ult_cnt = 0;

    aeb_cnt = (aeb_cnt + 1) % 100; // 100ms
    ult_cnt = (ult_cnt + 1) % 40; // 40ms

    if (aeb_cnt == 1)
    {
        // AEB
    }

    if (Get_APS_State() == 1)
    {

        if (ult_cnt == 1 || ult_cnt == 2)
        {
            // Ultrasonic sensor: Set the period to 40ms. 38ms(Max echo back pulse duration) + 2ms(Margin including trigger pulse)
            Toggle_Ultrasonic_Trigger();
        }
    }
}

void Run_Gpt12_T3 ()
{
    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}

void Stop_Gpt12_T3 ()
{
    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_stop);
}

void Gpt1_Init (void)
{
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    MODULE_GPT120.CLC.U = 0;
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());

    /* Initialize the Timer T3 */
    MODULE_GPT120.T3CON.B.T3M = 0x0; /* Set T3 to timer mode */
    MODULE_GPT120.T3CON.B.T3UD = 0x1; /* Set T3 count direction(down) */
    MODULE_GPT120.T3CON.B.BPS1 = GPT1_BLOCK_PRESCALER; /* Set GPT1 block prescaler */
    MODULE_GPT120.T3CON.B.T3I = TIMER_T3_INPUT_PRESCALER; /* Set T3 input prescaler */
    /* Calculate dutyUpTime and dutyDownTime for reloading timer T3 */
    MODULE_GPT120.T3.U = TIMER_T3_T2_VALUE; /* Set timer T3 value */
    /* Timer T2: reloads the value DutyDownTime in timer T3 */
    MODULE_GPT120.T2CON.B.T2M = 0x4; /* Set the timer T2 in reload mode */
    MODULE_GPT120.T2CON.B.T2I = 0x7; /* Reload Input Mode : Rising/Falling Edge T3OTL */
    MODULE_GPT120.T2.U = TIMER_T3_T2_VALUE;

    /* Initialize the interrupt */
    volatile Ifx_SRC_SRCR *src;
    src = (volatile Ifx_SRC_SRCR*) (&MODULE_SRC.GPT12.GPT12[0].T3);
    src->B.SRPN = ISR_PRIORITY_GPT1T3_TIMER;
    src->B.TOS = 0;
    src->B.CLRR = 1; /* clear request */

    src->B.SRE = 1; /* interrupt enable */

    Run_Gpt12_T3();
}
