#ifndef BSW_DRIVER_ISR_PRIORITY_H_
#define BSW_DRIVER_ISR_PRIORITY_H_

/* Definition of the interrupt priorities */
/* High value - higher priority */

#define ISR_PRIORITY_ASCLIN0_RX 18
#define ISR_PRIORITY_ASCLIN0_TX 19

#define ISR_PRIORITY_ASCLIN1_RX 4
//#define ISR_PRIORITY_ASCLIN1_TX 5

#define ISR_PRIORITY_ASCLIN2_RX 22
//#define ISR_PRIORITY_ASCLIN2_TX 23

#define ISR_PRIORITY_CAN_RX 0 /* Define the CAN RX interrupt priority */
#define ISR_PRIORITY_CAN_TX 0 /* Define the CAN TX interrupt priority */

//#define ISR_PRIORITY_CANFD_RX 8 /* Define the CAN RX interrupt priority */
//#define ISR_PRIORITY_CANFD_TX 9 /* Define the CAN TX interrupt priority */

#define ISR_PRIORITY_SCUERU0 50
#define ISR_PRIORITY_SCUERU1 51
#define ISR_PRIORITY_SCUERU2 52

#define ISR_PRIORITY_GPT1T3_TIMER 8 /* Define the GPT12 Timer interrupt priority  */

#endif /* BSW_DRIVER_ISR_PRIORITY_H_ */
