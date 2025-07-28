#include "aps.h"

volatile int aps_state = 0;

int Get_APS_State (void)
{
    return aps_state;
}

void Set_APS_State (int state)
{
    aps_state = state;
}

int Get_APS_Result (int *res_x, int *res_y)
{

    return 0;
}
