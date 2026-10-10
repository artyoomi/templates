#include <gtest/gtest.h>

#include "./reverse_polish_notation.h"

TEST(TestPolish, tests)
{
    EXPECT_EQ(to_polish(""), "");
    EXPECT_EQ("5", "5");
    EXPECT_EQ("3 + 5", "3 5 +");
    EXPECT_EQ("3 + 5 - 2", "3 5 + 2 -");
    EXPECT_EQ("3 + (5 - 2)", "3 5 2 - +");
    EXPECT_EQ("3 + 5 * 2", "3 5 2 * +");
    EXPECT_EQ("(3 + 5) * (2 - 1)", "3 5 + 2 1 - *");
    EXPECT_EQ("2 ^ 3", "2 3 ^");
    EXPECT_EQ("2 ^ 3 + 4", "2 3 ^ 4 +");
    EXPECT_EQ("2 + 3 * (4 - 5) / 6 ^ 7", "2 3 4 5 - * 6 7 ^ / +");
    EXPECT_EQ("3 + (4 * 2", "");
    EXPECT_EQ("3 + + 5", "");
    EXPECT_EQ("+", "");
    EXPECT_EQ("123456 + 789012", "123456 789012 +");
    EXPECT_EQ("1 + 2 - 3 + 4 * 5 / 6 ^ 7", "1 2 + 3 - 4 5 * 6 7 ^ / +");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}
