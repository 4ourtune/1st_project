#include <gtest/gtest.h>

// AEB (Autonomous Emergency Braking) function stubs
bool aeb_initialize() { return true; }
bool aeb_enable() { return true; }
bool aeb_disable() { return true; }
bool aeb_is_enabled() { return true; }
int aeb_get_distance_sensor_value() { return 150; }
bool aeb_set_threshold_distance(int distance) { return distance > 0 && distance <= 500; }
int aeb_get_threshold_distance() { return 100; }
bool aeb_emergency_brake_activated() { return false; }
bool aeb_trigger_emergency_brake() { return true; }

TEST(AEBTest, InitializeWorks) {
    EXPECT_TRUE(aeb_initialize());
}

TEST(AEBTest, EnableWorks) {
    EXPECT_TRUE(aeb_enable());
}

TEST(AEBTest, DisableWorks) {
    EXPECT_TRUE(aeb_disable());
}

TEST(AEBTest, IsEnabledWorks) {
    EXPECT_TRUE(aeb_is_enabled());
}

TEST(AEBTest, GetDistanceSensorValueWorks) {
    EXPECT_EQ(aeb_get_distance_sensor_value(), 150);
}

TEST(AEBTest, SetThresholdDistanceWorks) {
    EXPECT_TRUE(aeb_set_threshold_distance(100));
    EXPECT_TRUE(aeb_set_threshold_distance(500));
}

TEST(AEBTest, SetThresholdDistanceInvalidInput) {
    EXPECT_FALSE(aeb_set_threshold_distance(0));
    EXPECT_FALSE(aeb_set_threshold_distance(-10));
    EXPECT_FALSE(aeb_set_threshold_distance(501));
}

TEST(AEBTest, GetThresholdDistanceWorks) {
    EXPECT_EQ(aeb_get_threshold_distance(), 100);
}

TEST(AEBTest, EmergencyBrakeActivatedWorks) {
    EXPECT_FALSE(aeb_emergency_brake_activated());
}

TEST(AEBTest, TriggerEmergencyBrakeWorks) {
    EXPECT_TRUE(aeb_trigger_emergency_brake());
}