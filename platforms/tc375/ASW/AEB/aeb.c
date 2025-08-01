#include "aeb.h"
#include "ToF.h"
#include "motor_controller.h"
#include "my_stdio.h"

static volatile int aeb_state = AEB_STATE_NORMAL;

bool AEB_IsEmergencyBrakingRequired(void)
{
    unsigned int distance_mm = Tof_GetCorrectedDistance();

    //my_printf("[TOF] %4u mm \n", distance_mm);

    // 1순위: 50cm 이하면 무조건 긴급제동
    if (distance_mm <= 500)
    { // 50cm = 500mm
        return true;
    }

    int motorA_speed = MotorController_GetMotorChA();
    int motorB_speed = MotorController_GetMotorChB();

    int vehicle_speed = (motorA_speed + motorB_speed) / 2;

    if (vehicle_speed <= 0)
    {
        return false;
    }

    unsigned int braking_distance_mm = AEB_SPEED_COEFFICIENT * vehicle_speed * vehicle_speed / AEB_SPEED_DIVIDER;

    if (distance_mm <= (braking_distance_mm + AEB_SAFETY_MARGIN_MM))
    {
        return true;
    }

    return false;
}

void AEB_UpdateState(void)
{
    if (AEB_IsEmergencyBrakingRequired())
    {
        aeb_state = AEB_STATE_EMERGENCY;
    }
    else
    {
        aeb_state = AEB_STATE_NORMAL;
    }
}

int Get_AEB_State(void)
{
    return aeb_state;
}

void AEB_SetState(int state)
{
    if (state == AEB_STATE_NORMAL || state == AEB_STATE_EMERGENCY)
    {
        aeb_state = state;
    }
}

static int test_phase = 0;
static unsigned int test_start_distance = 0;
static unsigned int test_stop_distance = 0;
static int test_target_speed = 0;

void AEB_TestBrakingDistance(int target_speed)
{
    test_target_speed = target_speed;

    switch (test_phase)
    {
    case 0:
        test_start_distance = Tof_GetCorrectedDistance();
        test_phase = 1;
        break;

    case 1:
        if (Tof_GetCorrectedDistance() <= AEB_TRIGGER_DISTANCE_MM)
        {
            test_phase = 2;
        }
        break;

    case 2:
        test_stop_distance = Tof_GetCorrectedDistance();
        test_phase = 0;
        break;
    }
}

void AEB_ResetTestState(void)
{
    test_phase = 0;
    test_start_distance = 0;
    test_stop_distance = 0;
    test_target_speed = 0;
}
