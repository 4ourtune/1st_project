#ifndef BSW_DRIVER_GPIO_H_
#define BSW_DRIVER_GPIO_H_

/* Init functions */
void GPIO_Init (void);
void GPIO_SetLed (unsigned char num_LED, unsigned char onOff);
void GPIO_ToggleLed (unsigned char num_LED);

#endif /* BSW_DRIVER_GPIO_H_ */
