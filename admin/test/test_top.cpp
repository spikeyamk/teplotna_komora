#include <gtest/gtest.h>
#include "../src/returns_true.hpp"

TEST(top, returns_true_succeeds) {
    EXPECT_EQ(returns_true(), 1);
}

TEST(top, returns_true_fails) {
    EXPECT_EQ(returns_true(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}