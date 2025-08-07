#include "emer_light.h"
#include "GPIO.h"
#include "Buzzer.h"
#include "stm.h"
#include "my_stdio.h"

void Emer_Light_Init (void)
{
    GPIO_SetLed(1, 0);
    GPIO_SetLed(2, 0);
}

void Emer_Light_Blink_For_APS (int distance)
{
    // 디버깅용으로 거리 출력
    my_printf("[Emer_Light] Distance for APS: %d mm\n", distance);
    // 거리 값에 따라 LED와 부저를 깜빡이도록 설정
    if (distance < 200)
    {
        Emer_Light_BlinkFast();
    }
    else if (distance < 500)
    {
        Emer_Light_BlinkSlow();
    }
    else
    {
        Emer_Light_Off();
    }
}

bool Emer_Light_APS_DONE (void)
{
    static uint64 last_toggle_time_APS_DONE = 0;
    // static int led_on_fast = 0;  // LED 상태 추적
    uint64 now = getTimeMs();

    // led가 꺼져있으면 킨다
    if (GPIO_GetLed(1) == 0 && GPIO_GetLed(2) == 0)
    {
        GPIO_SetLed(1, 1);
        GPIO_SetLed(2, 1);
        Buzzer_On();   // LED 켜질 때 부저 ON
    }

    // 500ms 보고 끈다다
    if (now - last_toggle_time_APS_DONE >= 500000000)
    {
        GPIO_SetLed(1, 0);
        GPIO_SetLed(2, 0);
        Buzzer_Off();   // LED 꺼질 때 부저 OFF
        return true;    // APS 완료 상태를 나타내는 true 반환
    }

    return false; // 아직 APS 완료 상태가 아님
}

void Emer_Light_BlinkFast (void)
{
    static uint64 last_toggle_time_fast = 0;
    static int led_on_fast = 0;  // LED 상태 추적
    uint64 now = getTimeMs();

    if (now - last_toggle_time_fast >= 100) // 100ms 간격으로 깜빡임
    {
        GPIO_ToggleLed(1);
        GPIO_ToggleLed(2);

        // led_on_fast = !led_on_fast;

        if (GPIO_GetLed(1))
        {
            Buzzer_On();   // LED 켜질 때 부저 ON
        }
        else
        {
            Buzzer_Off();  // LED 꺼질 때 부저 OFF
        }

        last_toggle_time_fast = now;
    }
}

void Emer_Light_BlinkSlow (void)
{
    static uint64 last_toggle_time_slow = 0;
    static int led_on = 0;  // LED 상태 추적
    uint64 now = getTimeMs();

    if (now - last_toggle_time_slow >= 300) // 300ms 간격으로 깜빡임
    {
        GPIO_ToggleLed(1);
        GPIO_ToggleLed(2);

        // led_on = !led_on;

        if (GPIO_GetLed(1))
        {
            Buzzer_On();   // LED 켜질 때 부저 ON
        }
        else
        {
            Buzzer_Off();  // LED 꺼질 때 부저 OFF
        }

        last_toggle_time_slow = now;
    }
}

void Emer_Light_Blink (void)
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
            Buzzer_On();   // LED 켜질 때 부저 ON
        }
        else
        {
            Buzzer_Off();  // LED 꺼질 때 부저 OFF
        }

        last_toggle_time = now;
    }
}

void Emer_Light_Off (void)
{
    GPIO_SetLed(1, 0);
    GPIO_SetLed(2, 0);
}
