#ifndef BSW_DRIVER_GPIO_H_
#define BSW_DRIVER_GPIO_H_

#include "Ifx_reg.h"

void GPIO_Init (void);
void GPIO_SetBuzzer (int state);
void GPIO_ToggleBuzzer (void);
void GPIO_SetLed (unsigned char num_LED, unsigned char onOff);
void GPIO_ToggleLed (unsigned char num_LED);
void GPIO_SetUltTrig (int state);

#endif /* BSW_DRIVER_GPIO_H_ */
