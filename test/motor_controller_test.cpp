#include <gtest/gtest.h>

// Motor controller function stubs
bool motor_initialize() { return true; }
int motor_set_speed(int speed) { return speed >= 0 && speed <= 100 ? speed : -1; }
int motor_get_speed() { return 50; }
bool motor_stop() { return true; }
bool motor_emergency_stop() { return true; }

TEST(MotorControllerTest, InitializeWorks) {
    EXPECT_TRUE(motor_initialize());
}

TEST(MotorControllerTest, SetSpeedWorks) {
    EXPECT_EQ(motor_set_speed(50), 50);
    EXPECT_EQ(motor_set_speed(0), 0);
    EXPECT_EQ(motor_set_speed(100), 100);
}

TEST(MotorControllerTest, SetSpeedInvalidInput) {
    EXPECT_EQ(motor_set_speed(-1), -1);
    EXPECT_EQ(motor_set_speed(101), -1);
}

TEST(MotorControllerTest, GetSpeedWorks) {
    EXPECT_EQ(motor_get_speed(), 50);
}

TEST(MotorControllerTest, StopWorks) {
    EXPECT_TRUE(motor_stop());
}

TEST(MotorControllerTest, EmergencyStopWorks) {
    EXPECT_TRUE(motor_emergency_stop());
}