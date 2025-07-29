#include "motor_controller.h"
#include "../IO/Motor.h"

static int g_motorChA_Speed = 0;
static int g_motorChB_Speed = 0;
static int g_motorChA_Direction = 1;
static int g_motorChB_Direction = 1;

void MotorController_Init(void)
{
    g_motorChA_Speed = 0;
    g_motorChB_Speed = 0;
    g_motorChA_Direction = 1;
    g_motorChB_Direction = 1;
}

/*-100 <= speed <= 100*/
int MotorController_GetMotorChA(void)
{
    if (g_motorChA_Direction == 1){
        return g_motorChA_Speed;
    } else {
        return -g_motorChA_Speed;
    }
}

/*-100 <= speed <= 100*/
int MotorController_GetMotorChB(void)
{
    if (g_motorChB_Direction == 1){
        return g_motorChB_Speed;
    } else {
        return -g_motorChB_Speed;
    }
}

static void MotorController_SetMotorChA(int speed)
{
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    
    g_motorChA_Speed = (speed < 0) ? -speed : speed;
    g_motorChA_Direction = (speed >= 0) ? 1 : 0;
    
    if (speed == 0) {
        Motor_stopChA();
    } else {
        Motor_movChA_PWM(g_motorChA_Speed, g_motorChA_Direction);
    }
}

static void MotorController_SetMotorChB(int speed)
{
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    
    g_motorChB_Speed = (speed < 0) ? -speed : speed;
    g_motorChB_Direction = (speed >= 0) ? 1 : 0;
    
    if (speed == 0) {
        Motor_stopChB();
    } else {
        Motor_movChB_PWM(g_motorChB_Speed, g_motorChB_Direction);
    }
}

static void MotorController_SetSpeed(int left_speed, int right_speed)
{
    if (left_speed > 100) left_speed = 100;
    if (left_speed < -100) left_speed = -100;
    if (right_speed > 100) right_speed = 100;
    if (right_speed < -100) right_speed = -100;
    
    MotorController_SetMotorChA(right_speed);
    MotorController_SetMotorChB(left_speed);
}

static int MotorController_MapJoystickValue(int value)
{
    if (value < 0) value = 0;
    if (value > 99) value = 99;
    
    return (value * 200 / 99) - 100;
}

bool MotorController_ProcessJoystickInput(int x, int y)
{
    if (x < 0 || x >= 100 || y < 0 || y >= 100)
    {
        return false;
    }
    
    int x_speed = MotorController_MapJoystickValue(x);
    int y_speed = MotorController_MapJoystickValue(y);
    
    int left_speed = y_speed + x_speed;
    int right_speed = y_speed - x_speed;
    
    MotorController_SetSpeed(left_speed, right_speed);
    return true;
}

bool MotorController_ProcessWASDInput(char key)
{
   switch(key)
   {
       case 'w':
       case 'W':
           MotorController_SetSpeed(100, 100);     // Move forward: both motors same speed
           return true;
           
       case 's':
       case 'S':
           MotorController_SetSpeed(-100, -100);   // Move backward: both motors reverse
           return true;
           
       case 'a':
       case 'A':
           // Turn left while maintaining current direction
           if (g_motorChA_Direction > 0 && g_motorChB_Direction > 0) {
               // Turn left during forward movement
               MotorController_SetSpeed(50, 100);   
           } else if (g_motorChA_Direction < 0 && g_motorChB_Direction < 0) {
               // Turn left during backward movement (steering is reversed when backing up)
               MotorController_SetSpeed(-100, -50);  
           } else {
               // Pivot turn left when stationary
               MotorController_SetSpeed(-50, 50);
           }
           return true;
           
       case 'd':
       case 'D':
           // Turn right while maintaining current direction
           if (g_motorChA_Direction > 0 && g_motorChB_Direction > 0) {
               // Turn right during forward movement
               MotorController_SetSpeed(100, 50);   
           } else if (g_motorChA_Direction < 0 && g_motorChB_Direction < 0) {
               // Turn right during backward movement (steering is reversed when backing up)
               MotorController_SetSpeed(-50, -100);  
           } else {
               // Pivot turn right when stationary
               MotorController_SetSpeed(50, -50);
           }
           return true;
           
       case 'x':
       case 'X':
           MotorController_SetSpeed(0, 0);         // Stop: both motors stop
           return true;
           
       default:
           return false;    // Invalid command
   }
}
