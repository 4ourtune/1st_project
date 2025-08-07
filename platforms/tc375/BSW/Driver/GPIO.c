#include "GPIO.h"

void GPIO_Init (void)
{
    /* For buzzer */
    MODULE_P23.IOCR0.B.PC1 = 0x10;
    MODULE_P23.OUT.B.P1 = 0;

    /* For LED */
    MODULE_P21.IOCR4.B.PC4 = 0x10;
    MODULE_P21.IOCR0.B.PC0 = 0x10;
    MODULE_P21.OUT.B.P4 = 0;
    MODULE_P21.OUT.B.P0 = 0;

    /* For ultrasonic */
    MODULE_P10.IOCR4.B.PC4 = 0x10; // Set P10.4 as output
    MODULE_P10.OUT.B.P4 = 0;
}

void GPIO_SetBuzzer (int state)
{
    MODULE_P23.OUT.B.P1 = 0;
}

void GPIO_ToggleBuzzer (void)
{
    MODULE_P23.OUT.B.P1 ^= 1;
}

int GPIO_GetLed (unsigned char num_LED)
{
    switch (num_LED)
    {
        case 1 :
            return MODULE_P21.OUT.B.P4 ? 1 : 0;
        case 2 :
            return MODULE_P21.OUT.B.P0 ? 1 : 0;
        default :
            return -1; // 잘못된 번호
    }
}

void GPIO_SetLed (unsigned char num_LED, unsigned char onOff)
{
    switch (num_LED)
    {
        case 1 :
            MODULE_P21.OUT.B.P4 = onOff;
            break;
        case 2 :
            MODULE_P21.OUT.B.P0 = onOff;
            break;
    }
}

void GPIO_ToggleLed (unsigned char num_LED)
{
    switch (num_LED)
    {
        case 1 :
            MODULE_P21.OUT.B.P4 ^= 1;
            break;
        case 2 :
            MODULE_P21.OUT.B.P0 ^= 1;
            break;
    }
}

void GPIO_SetUltTrig (int state)
{
    MODULE_P10.OUT.B.P4 = state;
}
