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
