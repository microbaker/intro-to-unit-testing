#include <gtest/gtest.h>
#include <utility>

int add(int a, int b) { return a + b; }

class AddTest : public ::testing::TestWithParam<std::pair<int, int>> {};

TEST_P(AddTest, CorrectSum) {
  auto [a, b] = GetParam();
  EXPECT_EQ(add(a, b), a + b) << "a=" << a << " b=" << b;
}

INSTANTIATE_TEST_SUITE_P(
    SmallIntegers,
    AddTest,
    ::testing::Values(
        std::pair{0, 0},
        std::pair{1, 2},
        std::pair{-1, 1},
        std::pair{100, -50},
        std::pair{-10, -20}));

// Typed test example
template <typename T>
class NumericTest : public ::testing::Test {};

using NumericTypes = ::testing::Types<int, long, double>;
TYPED_TEST_SUITE(NumericTest, NumericTypes);

TYPED_TEST(NumericTest, ZeroIdentity) {
  TypeParam zero{};
  EXPECT_EQ(zero + zero, zero);
}
