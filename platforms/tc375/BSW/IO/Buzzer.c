#include "Buzzer.h"

void Buzzer_Init (void)
{
    /* Set P23.1 (Buzzer) as a general output pin */
    MODULE_P23.IOCR0.B.PC1 = 0x10;

    MODULE_P23.OUT.B.P1 = 0;

    Stop_Gpt12_T6();
}

void Buzzer_Buzz (void)
{
    static volatile unsigned int cntDelay = 0;
    cntDelay++;
    if (cntDelay % 500 == 0)
    {
        MODULE_P23.OUT.B.P1 ^= 1;
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
