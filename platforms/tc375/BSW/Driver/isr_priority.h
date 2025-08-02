#ifndef BSW_DRIVER_ISR_PRIORITY_H_
#define BSW_DRIVER_ISR_PRIORITY_H_

/* Definition of the interrupt priorities */
/* High value - higher priority */

#define ISR_PRIORITY_ASCLIN0_RX 18
#define ISR_PRIORITY_ASCLIN0_TX 19

#define ISR_PRIORITY_ASCLIN1_RX 4
//#define ISR_PRIORITY_ASCLIN1_TX 5

#define ISR_PRIORITY_CAN_RX 6 /* Define the CAN RX interrupt priority */
#define ISR_PRIORITY_CAN_TX 7 /* Define the CAN TX interrupt priority */

//#define ISR_PRIORITY_CANFD_RX 8 /* Define the CAN RX interrupt priority */
//#define ISR_PRIORITY_CANFD_TX 9 /* Define the CAN TX interrupt priority */

#define ISR_PRIORITY_SCUERU0 1
#define ISR_PRIORITY_SCUERU1 2
#define ISR_PRIORITY_SCUERU2 3

#define ISR_PRIORITY_GPT1T3_TIMER 8 /* Define the GPT12 Timer interrupt priority  */

#endif /* BSW_DRIVER_ISR_PRIORITY_H_ */
