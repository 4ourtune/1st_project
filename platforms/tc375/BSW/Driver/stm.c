#include "stm.h"

uint64 getTime10ns (void)
{
    /* Read 64-bit System Timer */
    uint32 upper1, lower, upper2;

    __dsync();
    do
    {
        upper1 = MODULE_STM0.TIM6.U;
        lower = MODULE_STM0.TIM0.U;
        upper2 = MODULE_STM0.TIM6.U;
    }while (upper1 != upper2);
    __isync();

    uint64 result = (((uint64) upper1) << 32) | lower;

    /* return 10nanoseconds */
    return result;
}

uint64 getTimeUs (void)
{
    /* return microseconds */
    uint64 result = getTime10ns();
    float32 frequency = 100000000.0f; // 100MHz -> 10^8Hz
    return result / (frequency / 1000000);
}

uint64 getTimeMs (void)
{
    /* return milliseconds */
    uint64 result = getTime10ns();
    float32 frequency = 100000000.0f;
    return result / (frequency / 1000);
}

IFX_INTERRUPT(Stm0IsrHandler, 0, ISR_PRIORITY_STM0);
void Stm0IsrHandler (void)
{
    GPIO_SetUltTrig(0);
}

void Stm0_Init (void)
{
    MODULE_STM0.CMCON.B.MSIZE0 = 31; /* Compare 32 bit size */
    MODULE_STM0.CMCON.B.MSTART0 = 0; /* Compare start at 0 bit */
    MODULE_STM0.ICR.B.CMP0OS = 0; /* Interrupt Output 0 */

    MODULE_SRC.STM.STM[0].SR[0].B.TOS = 0; /* CPU 0 */
    MODULE_SRC.STM.STM[0].SR[0].B.SRPN = ISR_PRIORITY_STM0;
    MODULE_SRC.STM.STM[0].SR[0].B.CLRR = 1; /* Clear Interrupt Req. */
    MODULE_SRC.STM.STM[0].SR[0].B.SRE = 1; /* Enable Interrupt */

    MODULE_STM0.ISCR.B.CMP0IRR = 1U; /* Clear Interrupt Req. */
    MODULE_STM0.ICR.B.CMP0EN = 1U; /* Enable Interrupt */
}

void Stm0_InterruptAfter (uint64 delay_us)
{
    /* Set Compare register to current time + delay_us */
    MODULE_STM0.CMP[0].U = (unsigned int) (getTimeUs() + delay_us) * 100;
}
