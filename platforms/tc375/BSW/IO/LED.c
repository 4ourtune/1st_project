#include "LED.h"

void LED_Init (void)
{
    GPIO_SetLed(1, 0);
    GPIO_SetLed(2, 0);
}

void LED_Toggle (void)
{
    GPIO_ToggleLed(1);
    GPIO_ToggleLed(2);
}

void LED_On (void)
{
    GPIO_SetLed(1, 1);
    GPIO_SetLed(2, 1);
}

void LED_Off (void)
{
    GPIO_SetLed(1, 0);
    GPIO_SetLed(2, 0);
}
