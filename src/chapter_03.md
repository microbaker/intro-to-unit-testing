# Chapter 3: ASSERT vs. EXPECT: Knowing the Difference

When a Google Test assertion fails, the framework must decide whether to continue executing the remainder of the current test function or to abort it immediately. The choice is expressed by the two families of macros: `EXPECT_*` (non-fatal) and `ASSERT_*` (fatal).

## 3.1 Assertions and Expectations Explained

Both families evaluate a condition and, on failure, record a failure with source location, the tested expression, and (for most macros) the expected and actual values. The sole behavioral difference is control flow:

- An `EXPECT_*` failure records the problem and continues executing the rest of the test function.
- An `ASSERT_*` failure records the problem and immediately returns from the current test function.

The macros otherwise accept identical arguments and support the same optional streamed messages.

## 3.2 When to Use ASSERT and EXPECT Macros

Use `EXPECT_*` by default. Most tests contain independent checks; continuing after a failure usually yields more diagnostic information.

Switch to `ASSERT_*` when later statements in the same test would be unsafe or meaningless if the assertion failed. Classic cases include:

- Verifying that a pointer is non-null before dereferencing it.
- Confirming that a resource was successfully obtained before using it.
- Establishing a precondition that every subsequent assertion depends upon.

## 3.3 Handling Failures and Their Impact

```cpp
int* maybe_allocate(bool succeed) {
  return succeed ? new int(42) : nullptr;
}

TEST(PointerTest, ExpectContinues) {
  int* p = maybe_allocate(false);
  EXPECT_NE(p, nullptr);
  EXPECT_EQ(*p, 42);  // may crash or fail confusingly
  delete p;
}

TEST(PointerTest, AssertAborts) {
  int* p = maybe_allocate(false);
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(*p, 42);  // never reached — safe
  delete p;
}
```

When the `ASSERT_NE` fails, Google Test reports the failure and ends the test function. The test binary itself continues with the next test case.

## 3.4 Effective Use of Messages in Assertions and Expectations

Both macro families accept an optional message streamed with `<<`.

```cpp
TEST(MathOperationsTest, SubtractWithMessage) {
  const int minuend = 3;
  const int subtrahend = 10;
  EXPECT_EQ(math::subtract(minuend, subtrahend), -7)
      << "minuend=" << minuend << " subtrahend=" << subtrahend;
}
```

## 3.5 Case Study: Leveraging ASSERT and EXPECT Macros

```cpp
#include <gtest/gtest.h>
#include "string_utils.h"

TEST(SafeSubstrTest, ExpectReportsMultipleFailures) {
  const std::string input = "hello";
  std::string result = safe_substr(input, 10, 2);

  EXPECT_FALSE(result.empty()) << "expected non-empty for valid range";
  EXPECT_EQ(result, "he");
}

TEST(SafeSubstrTest, AssertStopsOnPrecondition) {
  const std::string input = "hello";
  std::string result = safe_substr(input, 10, 2);

  ASSERT_FALSE(result.empty()) << "cannot continue if result is empty";
  EXPECT_EQ(result.size(), 2u);
}
```

## Common Pitfalls

- Using `ASSERT_*` for every check “just to be safe.”
- Using `EXPECT_*` before a dereference or other unsafe operation.
- Forgetting that both families support streamed messages.
- Assuming an `ASSERT_*` failure aborts the entire test binary.

## Chapter 3 Summary

`EXPECT_*` macros record a failure and continue; `ASSERT_*` macros record a failure and return from the current test function. Prefer `EXPECT_*` for independent checks. Use `ASSERT_*` when later statements depend on the check succeeding. Stream custom messages with `<<` on either macro.

## Questions and Exercises

1. In one sentence each, state what happens to control flow inside a test function when an `EXPECT_EQ` fails and when an `ASSERT_EQ` fails.
2. Why is `ASSERT_NE(ptr, nullptr)` preferable to `EXPECT_NE(ptr, nullptr)` immediately before a dereference of `ptr`?
3. Rewrite a test that checks a possibly-null pointer so that it uses the appropriate macro for the precondition.
4. True or false: An `ASSERT_*` failure prevents all subsequent test cases in the same binary from running. Explain.
