#include <gtest/gtest.h>
#include <stdexcept>
#include <cassert>

int safe_divide(int a, int b) {
  if (b == 0) throw std::invalid_argument("division by zero");
  return a / b;
}

TEST(ExceptionTest, ThrowsOnZero) {
  EXPECT_THROW(safe_divide(10, 0), std::invalid_argument);
  EXPECT_NO_THROW(safe_divide(10, 2));
}

void must_be_positive(int x) {
  assert(x > 0 && "value must be positive");
}

TEST(DeathTest, AbortsOnNonPositive) {
  EXPECT_DEATH(must_be_positive(0), "value must be positive");
  EXPECT_DEATH(must_be_positive(-5), "");
}

TEST(ValueTest, KnownSquares) {
  auto square = [](double x) { return x * x; };
  EXPECT_DOUBLE_EQ(square(5.0), 25.0);
  EXPECT_NEAR(square(1.414213562), 2.0, 1e-6);
}
