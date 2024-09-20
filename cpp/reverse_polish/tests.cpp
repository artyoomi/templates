#include <gtest/gtest.h>

#include "./to_polish.h"

TEST(TestPolist, tests)
{
    EXPECT_EQ(to_polish(""), "");
    EXPECT_EQ(to_polish("5"), "5");
    EXPECT_EQ(to_polish("3 + 5"), "3 5 +");
    EXPECT_EQ(to_polish("3 + 5 - 2"), "3 5 + 2 -");
    EXPECT_EQ(to_polish("3 + ( 5 - 2 )"), "3 5 2 - +");
    EXPECT_EQ(to_polish("3 + 5 * 2"), "3 5 2 * +");
    EXPECT_EQ(to_polish("( 3 + 5 ) * ( 2 - 1 )"), "3 5 + 2 1 - *");
    EXPECT_EQ(to_polish("2 ^ 3"), "2 3 ^");
    EXPECT_EQ(to_polish("2 ^ 3 + 4"), "2 3 ^ 4 +");
    EXPECT_EQ(to_polish("2 + 3 * ( 4 - 5 ) / 6 ^ 7"), "2 3 4 5 - * 6 7 ^ / +");
    // EXPECT_EQ(to_polish("3 + ( 4 * 2 "), "");
    EXPECT_EQ(to_polish("3 + + 5"), "");
    EXPECT_EQ(to_polish("+"), "");
    EXPECT_EQ(to_polish("123456 + 789012"), "");
    EXPECT_EQ(to_polish("1 + 2 - 3 + 4 * 5 / 6 ^ 7"), "");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
