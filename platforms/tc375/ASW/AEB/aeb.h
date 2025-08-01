#ifndef ASW_AEB_AEB_H_
#define ASW_AEB_AEB_H_

#include <stdbool.h>

#define AEB_STATE_NORMAL    0
#define AEB_STATE_EMERGENCY 1

#define AEB_SAFETY_MARGIN_MM    100
#define AEB_TRIGGER_DISTANCE_MM 500
#define AEB_SPEED_COEFFICIENT   5
#define AEB_SPEED_DIVIDER       100

bool AEB_IsEmergencyBrakingRequired(void);
void AEB_UpdateState(void);
int Get_AEB_State(void);

/*테스트 및 디버깅용*/
void AEB_SetState(int state);
void AEB_TestBrakingDistance(int target_speed);
void AEB_ResetTestState(void);

#endif /* ASW_AEB_AEB_H_ */
