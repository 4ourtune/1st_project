#include "Buzzer.h"

void Buzzer_Init (void)
{
    Stop_Gpt12_T6();
    GPIO_SetBuzzer(0);
}

void Buzzer_Buzz (void)
{
    static volatile unsigned int cntDelay = 0;
    cntDelay++;
    if (cntDelay % 500 == 0)
    {
        GPIO_ToggleBuzzer();
    }
}

void Buzzer_On (void)
{
    Run_Gpt12_T6();
}

void Buzzer_Off (void)
{
    Stop_Gpt12_T6();
}
