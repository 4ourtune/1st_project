#include "GPIO.h"
#include "Ifx_reg.h"

void GPIO_Init (void)
{
    MODULE_P21.IOCR4.B.PC4 = 0x10;
    MODULE_P21.IOCR0.B.PC0 = 0x10;

    MODULE_P21.OUT.B.P4 = 0;
    MODULE_P21.OUT.B.P0 = 0;
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
