#include "Ultrasonic.h"

void Ultrasonic_Trigger_Init (void)
{
    MODULE_P10.IOCR4.B.PC4 = 0x10; /* Set P10.4 as output */
    MODULE_P10.OUT.B.P4 = 0;
}

void Toggle_Ultrasonic_Trigger (void)
{
    MODULE_P10.OUT.B.P4 = !MODULE_P10.OUT.B.P4;
}
