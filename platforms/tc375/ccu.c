#include "ccu.h"

#define BUF_SIZE 16
#define MOTOR_STOP 50

void run_ccu (void)
{
    char user_cmd[BUF_SIZE];
    int pre_motor_y = MOTOR_STOP;
    int pre_motor_x = MOTOR_STOP;

    while (1)
    {
        int motor_y = MOTOR_STOP;
        int motor_x = MOTOR_STOP;
        int motor_valid = 0;

        if (queue_pull_string(user_cmd, BUF_SIZE))
        {
//            my_printf("%s\n", user_cmd);
            switch (user_cmd[0])
            {
                case 'M' :
                    Set_APS_State(0); // APS turns off when user inputs a command

                    // Set Motor
                    user_cmd[5] = '\0';
                    motor_y = atoi(user_cmd + 3);
                    user_cmd[3] = '\0';
                    motor_x = atoi(user_cmd + 1);

                    motor_valid = 1;

                    break;
                case 'P' :
                    Set_APS_State(1); // APS on
                    motor_valid = 1;

                    break;
                default : // Invalid command
                    motor_valid = 0;
                    break;
            }
        }

        if (Get_APS_State()) // If APS on
        {
            motor_valid = Get_APS_Result(&motor_x, &motor_y);
        }

        if (Get_AEB_State()) // If emergency break enabled
        {
            Set_APS_State(0); // APS off
            motor_valid = 1;

            if (!(pre_motor_x == MOTOR_STOP && pre_motor_y == MOTOR_STOP)) // If brake didn't work before
            {
                motor_x = motor_y = MOTOR_STOP;
            }
            else if (motor_y > MOTOR_STOP)
            {
                motor_valid = 0; // Can't move forward
            }
        }

        if (motor_valid)
        {
            my_printf("%d %d\n", motor_x, motor_y);
            int ret = MotorController_ProcessJoystickInput(motor_x, motor_y); // Controll motor
        }

        pre_motor_x = motor_x;
        pre_motor_y = motor_y;
    }
}
