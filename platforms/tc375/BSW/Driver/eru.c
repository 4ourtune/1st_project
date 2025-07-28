//eru.c
/*
 * scueru.c
 *
 *  Created on: 2024. 11. 6.
 *      Author: Kimjh
 */
#include "isr_priority.h"
#include <Ifx_reg.h>
#include <Ifx_Types.h>
#include <IfxScuWdt.h>
#include "my_stdio.h"
#include "eru.h"

void scueru_Init(void)
{
    uint16 password = IfxScuWdt_getSafetyWatchdogPasswordInline();
    IfxScuWdt_clearSafetyEndinitInline(password);
    MODULE_P13.IOCR0.B.PC2 = 0x10; // P13.2 - Rear TRIG
    MODULE_P15.IOCR0.B.PC3 = 0x10; // P15.3 - Right TRIG
    MODULE_P10.IOCR4.B.PC7 = 0x10; // P10.7 - Left TRIG

    /*** GPIO 입력 설정 (Echo 핀들) ***/
    MODULE_P13.IOCR0.B.PC1 = 0x02; // P13.1 - Rear sensor echo
    MODULE_P15.IOCR0.B.PC2 = 0x02; // P15.2 - Right sensor echo
    MODULE_P02.IOCR8.B.PC8 = 0x02; // P02.8 - Left sensor echo

    /*** Rear Sensor (P13.1 → EICR[0].Channel0) ***/
    MODULE_SCU.EICR[0].B.EXIS0 = 2;     // EXIS3 → P13.1
    MODULE_SCU.EICR[0].B.REN0  = 1;     // Rising edge enable
    MODULE_SCU.EICR[0].B.FEN0  = 1;     // Falling edge enable
    MODULE_SCU.EICR[0].B.EIEN0 = 1;     // Enable request
    MODULE_SCU.EICR[0].B.INP0  = 0;     // Connect to SRN0
    MODULE_SCU.IGCR[0].B.IGP0  = 1;     // Pattern match mode
    MODULE_SRC.SCU.SCUERU[0].B.SRPN = ISR_PRIORITY_ULTRASONIC_REAR;
    MODULE_SRC.SCU.SCUERU[0].B.TOS  = 0;  // CPU0
    MODULE_SRC.SCU.SCUERU[0].B.CLRR = 1;
    MODULE_SRC.SCU.SCUERU[0].B.SRE  = 1;

    /*** Right Sensor (P15.2 → EICR[0].Channel1) ***/
    MODULE_SCU.EICR[0].B.EXIS1 = 1;     // EXIS1 → P15.2
    MODULE_SCU.EICR[0].B.REN1  = 1;
    MODULE_SCU.EICR[0].B.FEN1  = 1;
    MODULE_SCU.EICR[0].B.EIEN1 = 1;
    MODULE_SCU.EICR[0].B.INP1  = 1;     // Connect to SRN1
    MODULE_SCU.IGCR[0].B.IGP1  = 1;
    MODULE_SRC.SCU.SCUERU[1].B.SRPN = ISR_PRIORITY_ULTRASONIC_RIGHT;
    MODULE_SRC.SCU.SCUERU[1].B.TOS  = 0;
    MODULE_SRC.SCU.SCUERU[1].B.CLRR = 1;
    MODULE_SRC.SCU.SCUERU[1].B.SRE  = 1;

    /*** Left Sensor (P02.8 → EICR[1].Channel0) ***/
    MODULE_SCU.EICR[1].B.EXIS0 = 2;     // EXIS2 → P02.8
    MODULE_SCU.EICR[1].B.REN0  = 1;
    MODULE_SCU.EICR[1].B.FEN0  = 1;
    MODULE_SCU.EICR[1].B.EIEN0 = 1;
    MODULE_SCU.EICR[1].B.INP0  = 2;     // Connect to SRN2
    MODULE_SCU.IGCR[1].B.IGP0  = 1;
    MODULE_SRC.SCU.SCUERU[2].B.SRPN = ISR_PRIORITY_ULTRASONIC_LEFT;
    MODULE_SRC.SCU.SCUERU[2].B.TOS  = 0;
    MODULE_SRC.SCU.SCUERU[2].B.CLRR = 1;
    MODULE_SRC.SCU.SCUERU[2].B.SRE  = 1;
    IfxScuWdt_setSafetyEndinitInline(password);

//    rear_sensor.start_time = 0; //////////////////////////
}

IFX_INTERRUPT(SCUERU_Int0_Handler, 0,ISR_PRIORITY_ULTRASONIC_REAR);
void SCUERU_Int0_Handler(void) {
    uint64 now = getTimeUs();
    my_printf("hanler 0 on");

//    if (P13_IN.B.P1 == 1) {  // Rising edge
//        rear_sensor.start_time = now;
//        my_printf("\nSTART %llu\n",rear_sensor.start_time);
//        rear_sensor.echo_state = 1;
//    } else {  // Falling edge
//        rear_sensor.end_time = now;
//        my_printf("\nEND %llu\n",rear_sensor.end_time);
//        my_printf("\n%llu %llu\n",rear_sensor.start_time,rear_sensor.end_time);
//        rear_sensor.duration = rear_sensor.end_time - rear_sensor.start_time;
//        rear_sensor.distance_cm = (0.0343f * rear_sensor.duration) / 2.0f;
//        rear_sensor.echo_state = 0;
//    }


    // 인터럽트 핸들러 내부
    if (rear_sensor.echo_state == 0) {
        // Rising edge
        rear_sensor.start_time = getTimeUs();
        rear_sensor.echo_state = 1;
        my_printf("START %llu\n", rear_sensor.start_time);
    } else {
        // Falling edge
        rear_sensor.end_time = getTimeUs();
        rear_sensor.duration = rear_sensor.end_time - rear_sensor.start_time;
        rear_sensor.distance_cm = (0.0343f * rear_sensor.duration) / 2.0f;
        rear_sensor.echo_state = 0;
        my_printf("END %llu\n", rear_sensor.end_time);
    }


}


IFX_INTERRUPT(SCUERU_Int1_Handler, 0, ISR_PRIORITY_ULTRASONIC_RIGHT);
void SCUERU_Int1_Handler(void) {
    uint64 now = getTimeUs();
    if (P15_IN.B.P2 == 1) {
        right_sensor.start_time = now;
        right_sensor.echo_state = 1;
    } else {
        right_sensor.end_time = now;
        right_sensor.duration = right_sensor.end_time - right_sensor.start_time;
        right_sensor.distance_cm = (0.0343f * right_sensor.duration) / 2.0f;
        right_sensor.echo_state = 0;
    }
    my_printf("hanler 1 on");
}

IFX_INTERRUPT(SCUERU_Int2_Handler, 0,ISR_PRIORITY_ULTRASONIC_LEFT);
void SCUERU_Int2_Handler(void) {
    uint64 now = getTimeUs();
    if (P02_IN.B.P8 == 1) {
        left_sensor.start_time = now;
        left_sensor.echo_state = 1;
    } else {
        left_sensor.end_time = now;
        left_sensor.duration = left_sensor.end_time - left_sensor.start_time;
        left_sensor.distance_cm = (0.0343f * left_sensor.duration) / 2.0f;
        left_sensor.echo_state = 0;
    }
    my_printf("hanler 2 on");
}
