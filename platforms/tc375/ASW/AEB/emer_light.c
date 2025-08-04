#include "emer_light.h"
#include "GPIO.h"
#include "stm.h"

void Emer_Light_Init(void)
{
    GPIO_SetLed(1, 0);
    GPIO_SetLed(2, 0);
}

void Emer_Light_Blink(void)
{
    static uint64 last_toggle_time = 0;
    uint64 now = getTimeMs();

    if (now - last_toggle_time >= 300)
    {
        GPIO_ToggleLed(1);
        GPIO_ToggleLed(2);
        last_toggle_time = now;
    }
}

void Emer_Light_Off(void)
{
    GPIO_SetLed(1, 0);
    GPIO_SetLed(2, 0);
}
