# Chapter 14: Custom Matchers, Listeners, and Richer Diagnostics

A well-written assertion already tells you what failed. Custom matchers and event listeners let you make that message speak the language of your domain and give you hooks to observe or record test execution without cluttering individual tests.

## 14.1 Writing Custom Matchers

Google Mock supplies a matcher framework. A custom matcher produces clearer failure text than a raw `EXPECT_TRUE` or a long streamed message.

```cpp
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <cmath>
#include <ostream>

MATCHER_P2(IsNearRelative, expected, rel_tol, "") {
  if (expected == 0.0) {
    *result_listener << "expected non-zero reference";
    return false;
  }
  const double diff = std::abs(arg - expected);
  const double limit = std::abs(expected) * rel_tol;
  if (diff <= limit) return true;
  *result_listener << "difference " << diff << " exceeds relative tolerance "
                   << rel_tol << " (limit " << limit << ")";
  return false;
}

TEST(MatcherDemo, RelativeTolerance) {
  EXPECT_THAT(1.01, IsNearRelative(1.0, 0.02));   // passes
  EXPECT_THAT(1.05, IsNearRelative(1.0, 0.02));   // fails with clear explanation
}
```

Matchers compose with `AllOf`, `AnyOf`, `Not`, and the built-in matchers (`Eq`, `Ge`, `Contains`, `Each`, etc.).

## 14.2 Defining Matchers for Your Types

```cpp
struct Point { int x, y; };

std::ostream& operator<<(std::ostream& os, const Point& p) {
  return os << "(" << p.x << "," << p.y << ")";
}

MATCHER_P(IsWithinDistance, origin, "") {
  const int dx = arg.x - origin.x;
  const int dy = arg.y - origin.y;
  const int dist2 = dx * dx + dy * dy;
  if (dist2 <= 25) return true;               // distance ≤ 5
  *result_listener << "distance squared " << dist2 << " > 25";
  return false;
}

TEST(PointTest, CloseToOrigin) {
  EXPECT_THAT((Point{3, 4}), IsWithinDistance(Point{0, 0}));
}
```

Once the streaming operator exists, ordinary `EXPECT_EQ` also prints the point usefully if you later add an `operator==`.

## 14.3 Event Listeners for Cross-Cutting Diagnostics

A listener receives callbacks for test-program start/end, each suite, and each test.

```cpp
#include <gtest/gtest.h>
#include <chrono>
#include <iostream>

class TimingListener : public ::testing::EmptyTestEventListener {
 public:
  void OnTestStart(const ::testing::TestInfo&) override {
    start_ = std::chrono::steady_clock::now();
  }
  void OnTestEnd(const ::testing::TestInfo& info) override {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::steady_clock::now() - start_)
                  .count();
    if (ms > 50) {
      std::cout << "[SLOW] " << info.test_suite_name() << "."
                << info.name() << " took " << ms << " ms\n";
    }
  }
 private:
  std::chrono::steady_clock::time_point start_;
};

// In main(), after InitGoogleTest:
// ::testing::UnitTest::GetInstance()->listeners().Append(new TimingListener);
```

Listeners are ideal for CI annotations, flaky-test detection hooks, or exporting results. Keep them lightweight.

## 14.4 Practical Guidelines and Pitfalls

- Prefer a custom matcher when the same complex condition appears in many tests or when the default failure text is unhelpful.
- Keep matcher descriptions precise; the text in `*result_listener` becomes part of the failure message.
- Listeners must be exception-safe and must not themselves call assertions that could recurse.
- Register listeners only in the test main (or a test-only translation unit).
- Matchers and listeners are optional power tools. Most suites remain clearer with ordinary `EXPECT_*` macros plus good messages.

## 14.5 Case Study: Domain-Specific Failure Messages

A geometry library repeatedly checked “point lies inside axis-aligned box.” Replacing a multi-line `EXPECT_TRUE` plus streamed coordinates with a single `EXPECT_THAT(p, IsInside(box))` made failures self-explanatory and removed duplicated diagnostic code. A short listener later flagged any test that allocated more than a threshold of memory, surfacing accidental issues during review.

## Chapter 14 Summary

Custom matchers give domain-specific, reusable failure explanations; event listeners provide a clean extension point for timing, logging, or reporting. Both stay outside production code and compose with the fixtures, parameterized tests, and CI practices of earlier chapters. Introduce them when they measurably reduce debugging time.

## Questions and Exercises

1. When is a custom matcher preferable to an `EXPECT_TRUE` plus a streamed message?
2. Write a matcher `IsEven()` that succeeds for even integers and, on failure, reports the actual value.
3. Add a streaming operator and a simple matcher for a `struct Version { int major, minor; }`.
4. What callbacks does `EmptyTestEventListener` provide, and which two would you override to measure per-test duration?
5. True or false: A listener can safely call `FAIL()` or `EXPECT_*` macros. Explain.
6. Name one risk of putting heavy work inside a listener.
