#include "aeb_alert.h"

static int alert_state = 0; // Alert 상태 추적

void AEBAlert_On (void)
{
    static uint64 last_toggle_time = 0;

    uint64 now = getTimeMs();

    if (now - last_toggle_time >= 300)
    {
        alert_state = !alert_state;

        if (alert_state)
        {
            // LED 켜질 때 부저 ON
            LED_On();
            Buzzer_On();
        }
        else
        {
            // LED 꺼질 때 부저 OFF
            LED_Off();
            Buzzer_Off();
        }

        last_toggle_time = now;
    }
}

void AEBAlert_Off (void)
{
    if (alert_state)
    {
        LED_Off();
        Buzzer_Off();
        alert_state = 0;
    }
}
