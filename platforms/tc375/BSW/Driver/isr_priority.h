#ifndef BSW_DRIVER_ISR_PRIORITY_H_
#define BSW_DRIVER_ISR_PRIORITY_H_

/* Definition of the interrupt priorities */
/* High value - higher priority */
#define ISR_PRIORITY_ASCLIN0_RX     18
#define ISR_PRIORITY_ASCLIN0_TX     19

#define ISR_PRIORITY_ASCLIN1_RX     4
//#define ISR_PRIORITY_ASCLIN1_TX     21

#define ISR_PRIORITY_CAN_TX         2       /* Define the CAN TX interrupt priority */
#define ISR_PRIORITY_CAN_RX         5       /* Define the CAN RX interrupt priority */

#define ISR_PRIORITY_CANFD_RX       3       /* Define the CAN RX interrupt priority */
#define ISR_PRIORITY_CANFD_TX       4       /* Define the CAN TX interrupt priority */

#define ISR_PRIORITY_ERU_INT0       17

#define ISR_PRIORITY_GPT1T3_TIMER   6       /* Define the GPT12 Timer interrupt priority  */

#endif /* BSW_DRIVER_ISR_PRIORITY_H_ */
