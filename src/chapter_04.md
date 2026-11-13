# Chapter 4: Adding Meaningful Messages to Test Results

Clear failure messages turn a red test into an immediately actionable diagnosis. Google Test already prints the assertion expression, expected value, and actual value; a short custom message adds the missing context.

## 4.1 Importance of Descriptive Failure Messages

A bare assertion failure tells you *what* failed. A well-crafted message tells you *why it matters* and *under which conditions*.

## 4.2 Enhancing Test Readability with Descriptive Messages

Any `EXPECT_*` or `ASSERT_*` macro accepts an optional message streamed with the `<<` operator. The message is evaluated only on failure.

```cpp
TEST(MathOperationsTest, AddWithContext) {
  const int a = 2;
  const int b = 2;
  EXPECT_EQ(math::add(a, b), 5)
      << "a=" << a << " b=" << b
      << " (positive integers should sum correctly)";
}
```

For loops or parameterized cases the message can include the current iteration or parameter:

```cpp
TEST(PrimeTest, SmallPrimes) {
  const int candidates[] = {2, 3, 4, 5};
  for (int n : candidates) {
    EXPECT_TRUE(is_prime(n)) << "n=" << n << " should be prime";
  }
}
```

## 4.3 Customizing Failure Messages for Better Diagnostics

Messages can incorporate any expression that streams to an `ostream`.

```cpp
std::string format_point(int x, int y) {
  return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}

TEST(GeometryTest, Distance) {
  const int x1 = 0, y1 = 0, x2 = 3, y2 = 4;
  EXPECT_NEAR(distance(x1, y1, x2, y2), 5.0, 1e-6)
      << "points " << format_point(x1, y1) << " and "
      << format_point(x2, y2);
}
```

## 4.4 Utilizing Color, XML, and JSON Formatting

Google Test can emit human-readable colored text, structured XML, or structured JSON.

```bash
./math_tests --gtest_color=yes
./math_tests --gtest_output=xml:test_results.xml
./math_tests --gtest_output=json:test_results.json
```

Custom messages appear in all three formats.

## Common Pitfalls

- Messages that merely restate the assertion.
- Side-effecting code inside the message expression.
- Extremely long messages that bury the expected/actual values.

## Chapter 4 Summary

Stream a short, contextual message onto any assertion with `<<`. The message appears in terminal output, XML reports, and JSON reports. Prefer messages that supply inputs or state over messages that merely restate the assertion.

## Questions and Exercises

1. What information does Google Test already print for a failed `EXPECT_EQ`, and what additional information can a custom message supply?
2. Write an assertion that checks `add(-1, 1) == 0` and includes a message showing both inputs.
3. Why is it useful that the streamed message expression is evaluated only when the assertion fails?
4. True or false: Custom messages appear only in colored terminal output and are omitted from XML and JSON reports. Explain.
