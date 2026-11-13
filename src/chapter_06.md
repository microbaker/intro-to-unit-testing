# Chapter 6: Value, Type, and Crash Testing

Beyond ordinary equality checks, Google Test provides direct support for validating specific values, exercising the same logic across types, verifying that exceptions are thrown, and confirming that certain code paths terminate the process.

## 6.1 Validating Specific Values and Types

**Value example:**

```cpp
#include <gtest/gtest.h>
#include <cmath>

double square(double x) { return x * x; }

TEST(ValueTest, SquareOfKnownValues) {
  EXPECT_DOUBLE_EQ(square(0.0), 0.0);
  EXPECT_DOUBLE_EQ(square(5.0), 25.0);
  EXPECT_DOUBLE_EQ(square(-3.0), 9.0);
  EXPECT_NEAR(square(1.414213562), 2.0, 1e-6);
}
```

**Type-parameterized example:**

```cpp
template <typename T>
class SquareTypedTest : public ::testing::Test {};

using ArithmeticTypes = ::testing::Types<int, long, double>;
TYPED_TEST_SUITE(SquareTypedTest, ArithmeticTypes);

TYPED_TEST(SquareTypedTest, ZeroAndPositive) {
  TypeParam zero{};
  TypeParam five{5};
  EXPECT_EQ(zero * zero, zero);
  EXPECT_EQ(five * five, TypeParam{25});
}
```

## 6.2 Handling Exceptions and Crashes in Unit Tests

**Exceptions:**

```cpp
#include <stdexcept>

int safe_divide(int a, int b) {
  if (b == 0) throw std::invalid_argument("division by zero");
  return a / b;
}

TEST(ExceptionTest, ThrowsOnZeroDivisor) {
  EXPECT_THROW(safe_divide(10, 0), std::invalid_argument);
}

TEST(ExceptionTest, DoesNotThrowOnValidInput) {
  EXPECT_NO_THROW(safe_divide(10, 2));
  EXPECT_EQ(safe_divide(10, 2), 5);
}
```

**Death tests:**

```cpp
#include <cassert>

void must_be_positive(int x) {
  assert(x > 0 && "value must be positive");
}

TEST(DeathTest, AbortsOnNonPositive) {
  EXPECT_DEATH(must_be_positive(0), "value must be positive");
  EXPECT_DEATH(must_be_positive(-1), "");
}
```

Death tests are controlled by `--gtest_death_test_style` (`fast` or `threadsafe`).

## 6.3 Ensuring Tests Fail Gracefully in Case of Crashes

- Prefer `EXPECT_*` over `ASSERT_*` unless a later statement would be unsafe.
- Use death tests for paths that are expected to terminate.
- Run the suite under AddressSanitizer or UndefinedBehaviorSanitizer in CI.
- Unit tests plus death tests are excellent for specified fatal paths but not a substitute for dynamic analysis tools.

## 6.4 Case Study: Thorough Testing with Value, Type, and Crash Tests

```cpp
#include <gtest/gtest.h>
#include <stdexcept>

int checked_factorial(int n) {
  if (n < 0) throw std::invalid_argument("negative");
  if (n > 12) std::abort();
  int result = 1;
  for (int i = 2; i <= n; ++i) result *= i;
  return result;
}

class FactorialValueTest : public ::testing::TestWithParam<std::pair<int, int>> {};

TEST_P(FactorialValueTest, KnownValues) {
  auto [input, expected] = GetParam();
  EXPECT_EQ(checked_factorial(input), expected) << "n=" << input;
}

INSTANTIATE_TEST_SUITE_P(Small, FactorialValueTest,
                         ::testing::Values(std::pair{0, 1},
                                           std::pair{1, 1},
                                           std::pair{5, 120},
                                           std::pair{7, 5040}));

TEST(FactorialExceptionTest, RejectsNegative) {
  EXPECT_THROW(checked_factorial(-1), std::invalid_argument);
}

TEST(FactorialDeathTest, AbortsOnExcessiveInput) {
  EXPECT_DEATH(checked_factorial(13), "");
}
```

## Common Pitfalls

- Using death tests for ordinary error returns.
- Forgetting that death tests execute in a child process.
- Over-reliance on unit tests to catch buffer overflows or leaks.
- Neglecting to test both the “throws” and “does not throw” sides of a contract.

## Chapter 6 Summary

Value tests lock down concrete results; typed tests confirm behavior across types; `EXPECT_THROW` verifies exception contracts; and death tests confirm that specified fatal paths actually terminate. Complement them with sanitizers for defects that pure unit assertions cannot observe.

## Questions and Exercises

1. What is the difference between `EXPECT_THROW` and `EXPECT_NO_THROW`?
2. Write a death test for a function that calls `std::abort()` on a null pointer.
3. Why do death tests run the checked statement in a subprocess?
4. True or false: A failing death test crashes the entire test binary. Explain.
5. When is AddressSanitizer more appropriate than a Google Test death test?
