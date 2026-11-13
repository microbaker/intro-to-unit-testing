# Chapter 5: Parameterized Testing

Parameterized tests let you write the test logic once and execute it against many inputs. They eliminate copy-and-paste duplication, make edge-case coverage systematic, and keep the suite maintainable when the set of interesting values grows.

## 5.1 Introduction to Parameterized Tests

A conventional test hard-codes one set of inputs. A parameterized test supplies a list (or generator) of inputs; the framework instantiates and runs the same test body for each element.

Benefits include:

- Reduced duplication
- Easier addition of new cases
- Systematic inclusion of boundaries and edge values
- Clearer intent

## 5.2 Writing Parameterized Tests Using Google Test

Value-parameterized tests derive from `::testing::TestWithParam<T>`. The test body is written with `TEST_P` and retrieves the current parameter with `GetParam()`.

```cpp
#include <gtest/gtest.h>
#include "math_operations.h"

class AddTest : public ::testing::TestWithParam<std::pair<int, int>> {};

TEST_P(AddTest, CorrectSum) {
  auto [a, b] = GetParam();
  EXPECT_EQ(math::add(a, b), a + b)
      << "a=" << a << " b=" << b;
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
```

Other useful generators include `ValuesIn`, `Range`, and `Combine`.

**Typed tests:**

```cpp
template <typename T>
class NumericAddTest : public ::testing::Test {};

using NumericTypes = ::testing::Types<int, long, double>;
TYPED_TEST_SUITE(NumericAddTest, NumericTypes);

TYPED_TEST(NumericAddTest, ZeroIdentity) {
  TypeParam zero{};
  EXPECT_EQ(zero + zero, zero);
}
```

## 5.3 Combining Parameterized Tests with Fixtures

```cpp
class MultiplyFixture : public ::testing::TestWithParam<int> {
 protected:
  void SetUp() override {
    scale_ = 2;
  }
  int scale_;
};

TEST_P(MultiplyFixture, ScalesCorrectly) {
  const int value = GetParam();
  EXPECT_EQ(value * scale_, value * 2);
}

INSTANTIATE_TEST_SUITE_P(PositiveValues, MultiplyFixture,
                         ::testing::Values(1, 5, 10, 100));
```

## 5.4 Managing Test Data for Parameterized Tests

- Prefer named constants or small helper functions for long lists.
- Use `ValuesIn` with a static array for large data.
- Group related parameters logically with separate instantiations.
- Give every instantiation a descriptive prefix.

## 5.5 Case Study: Efficient Testing with Parameterized Tests

```cpp
bool is_even(int n) { return n % 2 == 0; }

class EvenTest : public ::testing::TestWithParam<int> {};

TEST_P(EvenTest, IdentifiesEvenNumbers) {
  EXPECT_TRUE(is_even(GetParam())) << "value=" << GetParam();
}

INSTANTIATE_TEST_SUITE_P(Evens, EvenTest,
                         ::testing::Values(0, 2, 4, -2, 100));
```

## Common Pitfalls

- Instantiating with an empty parameter list.
- Forgetting that `GetParam()` is available only inside `TEST_P` or fixture methods.
- Mixing unrelated scenarios in a single giant parameter list.
- Neglecting a descriptive instantiation prefix.

## Chapter 5 Summary

Parameterized tests execute one body against many inputs or types, eliminating duplication while improving coverage and maintainability. Combine them with fixtures when setup is shared. Keep parameter data readable by extracting long lists and grouping related cases.

## Questions and Exercises

1. What problem does a value-parameterized test solve that a set of ordinary `TEST` macros does not?
2. Write a parameterized test for `math::subtract` that covers at least four pairs, including a message that prints both operands.
3. Explain the role of the first argument to `INSTANTIATE_TEST_SUITE_P`.
4. True or false: `SetUp()` in a `TestWithParam` fixture runs once per parameter value. Explain.
5. When would you choose a typed test over a value-parameterized test?
