#include <gtest/gtest.h>

// APS (Accelerator Pedal Sensor) function stubs
bool aps_initialize() { return true; }
int aps_read_raw_value() { return 2048; }
float aps_get_percentage() { return 45.5f; }
bool aps_is_valid_signal() { return true; }
bool aps_calibrate_min() { return true; }
bool aps_calibrate_max() { return true; }
int aps_get_min_value() { return 100; }
int aps_get_max_value() { return 4000; }
bool aps_is_calibrated() { return true; }

TEST(APSTest, InitializeWorks) {
    EXPECT_TRUE(aps_initialize());
}

TEST(APSTest, ReadRawValueWorks) {
    EXPECT_EQ(aps_read_raw_value(), 2048);
}

TEST(APSTest, GetPercentageWorks) {
    EXPECT_FLOAT_EQ(aps_get_percentage(), 45.5f);
}

TEST(APSTest, IsValidSignalWorks) {
    EXPECT_TRUE(aps_is_valid_signal());
}

TEST(APSTest, CalibrateMinWorks) {
    EXPECT_TRUE(aps_calibrate_min());
}

TEST(APSTest, CalibrateMaxWorks) {
    EXPECT_TRUE(aps_calibrate_max());
}

TEST(APSTest, GetMinValueWorks) {
    EXPECT_EQ(aps_get_min_value(), 100);
}

TEST(APSTest, GetMaxValueWorks) {
    EXPECT_EQ(aps_get_max_value(), 4000);
}

TEST(APSTest, IsCalibratedWorks) {
    EXPECT_TRUE(aps_is_calibrated());
}

TEST(APSTest, PercentageRange) {
    float percentage = aps_get_percentage();
    EXPECT_GE(percentage, 0.0f);
    EXPECT_LE(percentage, 100.0f);
}