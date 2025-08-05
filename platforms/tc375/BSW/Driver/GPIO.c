/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include <GPIO.h>
#include "Ifx_reg.h"

void GPIO_Init(void)
{
    /* Set P10.1 (LED2) & P10.2(LED1) as a general output pin */
    MODULE_P21.IOCR4.B.PC4 = 0x10;
    MODULE_P21.IOCR0.B.PC0 = 0x10;

    /* Turn off led */
    GPIO_SetLed(1, 0);
    GPIO_SetLed(2, 0);

    /* Set P2.0(SW1) * P2.1(SW2) as input pull-up */
//    MODULE_P02.IOCR0.B.PC0 = 0x02;
//    MODULE_P02.IOCR0.B.PC1 = 0x02;

    /* Set P00.7 (TC375 LED1) as a input pull-up */
//    MODULE_P00.IOCR4.B.PC7 = 0x02;
}

void GPIO_SetLed(unsigned char num_LED, unsigned char onOff)
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

void GPIO_ToggleLed(unsigned char num_LED)
{
    switch (num_LED)
    {
        case 1:
            MODULE_P21.OUT.B.P4 ^= 1;
            break;
        case 2:
            MODULE_P21.OUT.B.P0 ^= 1;
            break;
    }
}
