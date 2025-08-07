#include <gtest/gtest.h>

// example dummy function
int dummy_add(int a, int b) { return a + b; }
int dummy_sub(int a, int b) { return a - b; }

TEST(ExampleTest, AddWorks) {
    EXPECT_EQ(dummy_add(2, 3), 5);
}

TEST(Example2Test, SubWorks) {
    EXPECT_EQ(dummy_sub(5, 3), 2);
}
