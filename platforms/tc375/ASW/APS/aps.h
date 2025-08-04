#ifndef ASW_APS_APS_H_
#define ASW_APS_APS_H_

#include "ToF.h"
#include "Ultrasonic.h"
#include "stm.h"

//void Calc_APS_Result (void);
int Update_APS_Result (uint64 interval_us);
void Get_APS_Result (int *res_x, int *res_y, int *is_done);
int Get_APS_State (void);
void Set_APS_State (int state);

#endif /* ASW_APS_APS_H_ */
