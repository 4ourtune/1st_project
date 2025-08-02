#ifndef BSW_IO_TOF_H_
#define BSW_IO_TOF_H_

#include "stm.h"

void TofHandler (char *rxData, int rxLen);
void Tof_GetDistance (unsigned int *dist_mm, unsigned short *signal, unsigned char *status, uint64 *timestamp_us);
unsigned int Tof_GetCorrectedDistance (void);

#endif /* BSW_IO_TOF_H_ */
