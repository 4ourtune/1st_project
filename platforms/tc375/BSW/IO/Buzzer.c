#include "Ifx_reg.h"
#include "Buzzer.h"
#include "isr_priority.h"
#include "asclin.h"
#include "stm.h"
#include "Gpt12.h"


void Buzzer_Init(void)
{
    /* Set P02.3 (Buzzer) as a general output pin */
    MODULE_P23.IOCR0.B.PC1 = 0x10;
}
void Buzzer_off(void){
    stopGpt12_T6();

}
void Buzzer_on(void){
    runGpt12_T6();
}
