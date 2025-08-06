#include <emer_light.h>
#include <GPIO.h>
#include "Buzzer.h"
#include "stm.h"


void Emer_Light_Init (void)
{
    GPIO_SetLed(1, 0);
    GPIO_SetLed(2, 0);
}



void Emer_Light_Blink(void)
{
    static uint64 last_toggle_time = 0;
    static int led_on = 0;  // LED 상태 추적
    uint64 now = getTimeMs();

    if (now - last_toggle_time >= 300)
    {
        GPIO_ToggleLed(1);
        GPIO_ToggleLed(2);

        led_on = !led_on;

        if (led_on)
        {
            Buzzer_on();   // LED 켜질 때 부저 ON
        }
        else
        {
            Buzzer_off();  // LED 꺼질 때 부저 OFF
        }

        last_toggle_time = now;
    }
}



void Emer_Light_Off (void)
{
    GPIO_SetLed(1, 0);
    GPIO_SetLed(2, 0);
}
