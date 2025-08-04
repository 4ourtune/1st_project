#include "ccu.h"

#define BUF_SIZE 8
#define MOTOR_STOP 50
#define CYCLE_INTERVAL_US 40000 // 40000us = 40ms

void run_ccu (void)
{
    char user_cmd[BUF_SIZE];
    int pre_motor_x = MOTOR_STOP;
    int pre_motor_y = MOTOR_STOP;
    uint64 aeb_last_updated_time = 0;
    uint64 aps_last_updated_time = 0;

    while (1)
    {
        int motor_x = pre_motor_x;
        int motor_y = pre_motor_y;

        /* Check user commands */
        /* Command priority: 2 (High value - higher priority) */
        if (bluetooth_rx_queue_pull_string(user_cmd, BUF_SIZE))
        {
//            my_printf("%s\n", user_cmd);
            switch (user_cmd[0])
            {
                case 'M' : // 'Move'
                    Set_APS_State(0); // APS turns off when user inputs a 'Move' command

                    // Set motor inputs
                    user_cmd[5] = '\0';
                    motor_y = atoi(user_cmd + 3);
                    user_cmd[3] = '\0';
                    motor_x = atoi(user_cmd + 1);

                    break;

                case 'P' : // 'Automatic parking'
                    Set_APS_State(1); // APS on

                    break;

                default : // Invalid command
                    break;
            }
        }

        /* Check AEB */
        /* Command priority: 3 */
        int aeb_state_updated = call_after_interval(AEB_UpdateState, &aeb_last_updated_time,
        CYCLE_INTERVAL_US);
        if (AEB_GetState())
        {
            Set_APS_State(0); // APS off

            if (motor_y > MOTOR_STOP) // If it moves forward
            {
                motor_x = motor_y = MOTOR_STOP;
            }
        }

        /* Check APS */
        /* Command priority: 1 */
        if (Get_APS_State())
        {
            int aps_result_updated = call_after_interval(Update_APS_Result, &aps_last_updated_time,
            CYCLE_INTERVAL_US);

            if (aps_result_updated)
            {
                int is_done;
                Get_APS_Result(&motor_x, &motor_y, &is_done);
                if (is_done)
                {
                    Set_APS_State(0); // Terminate APS when it's done or has problem
                }
            }
        }

        /* Check motor control input */
        if (!(motor_x == pre_motor_x && motor_y == pre_motor_y))
        {
//            my_printf("%d %d\n", motor_x, motor_y);
            int success = MotorController_ProcessJoystickInput(motor_x, motor_y); // Controll motor

            if (success)
            {
                pre_motor_x = motor_x;
                pre_motor_y = motor_y;
            }
        }
    }
}
