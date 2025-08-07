#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "Ifx_Types.h"
#include <stdbool.h>

void MotorController_Init(void);
bool MotorController_ProcessJoystickInput(int x, int y);
bool MotorController_ProcessWASDInput(char key);

int MotorController_GetMotorChA(void);
int MotorController_GetMotorChB(void);
#endif /* MOTOR_CONTROLLER_H */
