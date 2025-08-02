#include "eru.h"

#define FILTER_WINDOW_SIZE 5
#define CALCULATE_DISTANCE(fall_time, rise_time) (0.343F * ((fall_time) - (rise_time)) / 2.0F) // Speed of sound: 0.343 mm/us

static AverageFilter filter[3];
static float ult_dist_mm[3];
static uint64 ult_timestamp_us[3];

IFX_INTERRUPT(SCUERU_Int0_Handler, 0, ISR_PRIORITY_SCUERU0);
void SCUERU_Int0_Handler (void)
{
    static uint64 rise_time;

    int input_pin_state = MODULE_P15.IN.B.P4;
    if (input_pin_state == 1)
    {
        rise_time = getTimeUs();
    }
    else if (input_pin_state == 0)
    {
        uint64 fall_time = getTimeUs();
        float distance_nofilt = CALCULATE_DISTANCE(fall_time, rise_time);
        float distance_filt = Filter_Update(filter + 0, distance_nofilt);

        ult_dist_mm[0] = distance_filt;
        ult_timestamp_us[0] = rise_time;
    }
}

IFX_INTERRUPT(SCUERU_Int1_Handler, 0, ISR_PRIORITY_SCUERU1);
void SCUERU_Int1_Handler (void)
{
    static uint64 rise_time;

    int input_pin_state = MODULE_P15.IN.B.P5;
    if (input_pin_state == 1)
    {
        rise_time = getTimeUs();
    }
    else if (input_pin_state == 0)
    {
        uint64 fall_time = getTimeUs();
        float distance_nofilt = CALCULATE_DISTANCE(fall_time, rise_time);
        float distance_filt = Filter_Update(filter + 1, distance_nofilt);

        ult_dist_mm[1] = distance_filt;
        ult_timestamp_us[1] = rise_time;
    }
}

IFX_INTERRUPT(SCUERU_Int2_Handler, 0, ISR_PRIORITY_SCUERU2);
void SCUERU_Int2_Handler (void)
{
    static uint64 rise_time;

    int input_pin_state = MODULE_P02.IN.B.P0;
    if (input_pin_state == 1)
    {
        rise_time = getTimeUs();
    }
    else if (input_pin_state == 0)
    {
        uint64 fall_time = getTimeUs();
        float distance_nofilt = CALCULATE_DISTANCE(fall_time, rise_time);
        float distance_filt = Filter_Update(filter + 2, distance_nofilt);

        ult_dist_mm[2] = distance_filt;
        ult_timestamp_us[2] = rise_time;
    }
}

void scueru_Init0 (void)
{
    uint16 password = IfxScuWdt_getSafetyWatchdogPasswordInline();
    IfxScuWdt_clearSafetyEndinitInline(password);

    MODULE_P15.IOCR4.B.PC4 = 0x02; /* Set P15.4 as pull-up input */

    /* EICR.EXIS 레지스터 설정 */
    MODULE_SCU.EICR[0].B.EXIS0 = 0;
    /* rising, falling edge 트리거 설정 */
    MODULE_SCU.EICR[0].B.REN0 = 1;
    MODULE_SCU.EICR[0].B.FEN0 = 1;
    /* Enable Trigger Pulse */
    MODULE_SCU.EICR[0].B.EIEN0 = 1;
    /* Determination of output channel for trigger event (Register INP) */
    MODULE_SCU.EICR[0].B.INP0 = 0;
    /* Configure Output channels, outputgating unit OGU (Register IGPy) */
    MODULE_SCU.IGCR[0].B.IGP0 = 1;

    volatile Ifx_SRC_SRCR *src;
    src = (volatile Ifx_SRC_SRCR*) (&MODULE_SRC.SCU.SCUERU[0]);
    src->B.SRPN = ISR_PRIORITY_SCUERU0;
    src->B.TOS = 0;
    src->B.CLRR = 1; /* clear request */
    src->B.SRE = 1; /* interrupt enable */

    IfxScuWdt_setSafetyEndinitInline(password);

    Filter_Init(filter + 0, FILTER_WINDOW_SIZE);
}

void scueru_Init1 (void)
{
    uint16 password = IfxScuWdt_getSafetyWatchdogPasswordInline();
    IfxScuWdt_clearSafetyEndinitInline(password);

    MODULE_P15.IOCR4.B.PC5 = 0x02; /* Set P15.5 as pull-up input */

    /* EICR.EXIS 레지스터 설정 */
    MODULE_SCU.EICR[2].B.EXIS0 = 3;
    /* rising, falling edge 트리거 설정 */
    MODULE_SCU.EICR[2].B.REN0 = 1;
    MODULE_SCU.EICR[2].B.FEN0 = 1;
    /* Enable Trigger Pulse */
    MODULE_SCU.EICR[2].B.EIEN0 = 1;
    /* Determination of output channel for trigger event (Register INP) */
    MODULE_SCU.EICR[2].B.INP0 = 1;
    /* Configure Output channels, outputgating unit OGU (Register IGPy) */
    MODULE_SCU.IGCR[0].B.IGP1 = 1;

    volatile Ifx_SRC_SRCR *src;
    src = (volatile Ifx_SRC_SRCR*) (&MODULE_SRC.SCU.SCUERU[1]);
    src->B.SRPN = ISR_PRIORITY_SCUERU1;
    src->B.TOS = 0;
    src->B.CLRR = 1; /* clear request */
    src->B.SRE = 1; /* interrupt enable */

    IfxScuWdt_setSafetyEndinitInline(password);

    Filter_Init(filter + 1, FILTER_WINDOW_SIZE);
}

void scueru_Init2 (void)
{
    uint16 password = IfxScuWdt_getSafetyWatchdogPasswordInline();
    IfxScuWdt_clearSafetyEndinitInline(password);

    MODULE_P02.IOCR0.B.PC0 = 0x02; /* Set P02.0 as pull-up input */

    /* EICR.EXIS 레지스터 설정 */
    MODULE_SCU.EICR[1].B.EXIS1 = 2;
    /* rising, falling edge 트리거 설정 */
    MODULE_SCU.EICR[1].B.REN1 = 1;
    MODULE_SCU.EICR[1].B.FEN1 = 1;
    /* Enable Trigger Pulse */
    MODULE_SCU.EICR[1].B.EIEN1 = 1;
    /* Determination of output channel for trigger event (Register INP) */
    MODULE_SCU.EICR[1].B.INP1 = 2;
    /* Configure Output channels, outputgating unit OGU (Register IGPy) */
    MODULE_SCU.IGCR[1].B.IGP0 = 1;

    volatile Ifx_SRC_SRCR *src;
    src = (volatile Ifx_SRC_SRCR*) (&MODULE_SRC.SCU.SCUERU[2]);
    src->B.SRPN = ISR_PRIORITY_SCUERU2;
    src->B.TOS = 0;
    src->B.CLRR = 1; /* clear request */
    src->B.SRE = 1; /* interrupt enable */

    IfxScuWdt_setSafetyEndinitInline(password);

    Filter_Init(filter + 2, FILTER_WINDOW_SIZE);
}

void get_eru_int0_data (unsigned int *dist_mm, uint64 *timestamp_us)
{
//    __disable();
    *dist_mm = (unsigned int) ult_dist_mm[0];
    *timestamp_us = ult_timestamp_us[0];
//    __enable();
}

void get_eru_int1_data (unsigned int *dist_mm, uint64 *timestamp_us)
{
//    __disable();
    *dist_mm = (unsigned int) ult_dist_mm[1];
    *timestamp_us = ult_timestamp_us[1];
//    __enable();
}

void get_eru_int2_data (unsigned int *dist_mm, uint64 *timestamp_us)
{
//    __disable();
    *dist_mm = (unsigned int) ult_dist_mm[2];
    *timestamp_us = ult_timestamp_us[2];
//    __enable();
}
