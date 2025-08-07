#ifndef BSW_IO_EMER_LIGHT_H_
#define BSW_IO_EMER_LIGHT_H_

#include <stdbool.h>

void Emer_Light_Init (void);
void Emer_Light_Blink_For_APS (int distance);
void Emer_Light_BlinkFast (void);
void Emer_Light_BlinkSlow (void);
void Emer_Light_Blink (void);
void Emer_Light_Off (void);
bool Emer_Light_APS_DONE (void);

#endif /* BSW_IO_EMER_LIGHT_H_ */
