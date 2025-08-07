#ifndef BSW_DRIVER_ASCLIN_H_
#define BSW_DRIVER_ASCLIN_H_

#include "IfxAsclin_Asc.h"
#include "IfxAsclin_bf.h"
#include "IfxCpu_Irq.h"

#include "isr_priority.h"

#include "Bluetooth.h"
#include "ToF.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ASCLIN0 for micro 5-pin serial communication */
void Asclin0_InitUart (void);
void Asclin0_OutUart (const unsigned char chr);
unsigned char Asclin0_InUart (void);
unsigned char Asclin0_InUartNonBlock (void);
int Asclin0_PollUart (unsigned char *chr);

/* ASCLIN1 for Bluetooth */
void Asclin1_InitUart (void);
void Asclin1_OutUart (const unsigned char chr);
unsigned char Asclin1_InUart (void);
unsigned char Asclin1_InUartNonBlock (void);
int Asclin1_PollUart (unsigned char *chr);

/* ASCLIN2 for ToF */
void Asclin2_InitUart (void);
void Asclin2_OutUart (const unsigned char chr);
unsigned char Asclin2_InUart (void);
unsigned char Asclin2_InUartNonBlock (void);
int Asclin2_PollUart (unsigned char *chr);

#endif /* BSW_DRIVER_ASCLIN_H_ */
