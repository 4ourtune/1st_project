#ifndef BSW_IO_BUZZER_H_
#define BSW_IO_BUZZER_H_

#include "GPIO.h"
#include "gpt12.h"

void Buzzer_Init (void);
void Buzzer_Buzz (void);
void Buzzer_On (void);
void Buzzer_Off (void);

#endif /* BSW_IO_BUZZER_H_ */
