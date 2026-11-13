#include <gtest/gtest.h>
#include "math_operations.h"

TEST(MathOperationsTest, AddPositiveNumbers) {
  EXPECT_EQ(math::add(2, 3), 5);
  EXPECT_EQ(math::add(0, 0), 0);
}

TEST(MathOperationsTest, AddNegativeNumbers) {
  EXPECT_EQ(math::add(-1, -1), -2);
  EXPECT_EQ(math::add(-5, 3), -2);
}

TEST(MathOperationsTest, Subtract) {
  EXPECT_EQ(math::subtract(10, 4), 6);
  EXPECT_EQ(math::subtract(0, 5), -5);
}

TEST(MathOperationsTest, Multiply) {
  EXPECT_EQ(math::multiply(3, 4), 12);
  EXPECT_EQ(math::multiply(-2, 5), -10);
  EXPECT_EQ(math::multiply(0, 100), 0);
}
