# Chapter 12: Advanced Testing Techniques (Optional)

The techniques in this chapter are not required for every code base. They become valuable when dependencies are heavy, when the input space is large, or when rare paths carry high risk.

## 12.1 Mocking Dependencies Using Google Mock

Google Mock lets you replace a dependency with a controllable test double.

```cpp
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

class DataSource {
 public:
  virtual ~DataSource() = default;
  virtual std::string fetch(int id) = 0;
};

class Report {
 public:
  explicit Report(DataSource& source) : source_(source) {}
  std::string title_for(int id) { return "Title: " + source_.fetch(id); }
 private:
  DataSource& source_;
};

class MockDataSource : public DataSource {
 public:
  MOCK_METHOD(std::string, fetch, (int id), (override));
};

TEST(ReportTest, UsesFetchedValue) {
  MockDataSource mock;
  EXPECT_CALL(mock, fetch(42))
      .WillOnce(::testing::Return("Answer"));

  Report report(mock);
  EXPECT_EQ(report.title_for(42), "Title: Answer");
}
```

Link with `GTest::gmock_main`. Use mocks when the real dependency is slow, non-deterministic, or unavailable. Prefer fakes for simple interactions.

## 12.2 Exploring Property-Based Testing

Property-based testing generates many inputs automatically and checks that a property holds for all of them. In C++ common libraries include RapidCheck. A property is a predicate that should be true for every input in a domain.

Identify pure functions with clear algebraic properties. Start with hand-written boundary cases, then add a property-based check. Keep the property tests in the same suite so they run under normal CI.

## 12.3 Handling Edge Cases and Rare Scenarios Effectively

Edge cases include empty containers, maximum and minimum numeric values, null or empty strings, and off-by-one boundaries.

Techniques that stay inside Google Test:

- Parameterized tests whose value list explicitly contains the edges.
- Dedicated test cases named for the edge.
- Death tests or `EXPECT_THROW` for rare fatal or exceptional paths.
- Coverage reports to confirm the edge branches are taken.

## 12.4 Leveraging Advanced Techniques for Complex Code

Apply mocking when a unit has several external collaborators. Apply property-based testing when the specification is best stated as an invariant. Apply systematic edge-case lists when historical defects clustered around boundaries. Introduce the techniques only where they reduce risk or effort.

## 12.5 Case Study: Advanced Techniques in Action

A pricing service depends on a remote tax calculator. A mock replaces the tax calculator. A parameterized test supplies edge values. A property-style check confirms the final price is never negative for non-negative inputs.

## Common Pitfalls

- Over-mocking internal details so tests break on harmless refactoring.
- Property-based tests whose properties are too weak to be useful.
- Ignoring shrinking output from property-based tools.
- Adding advanced machinery to code that would be clearer with ordinary tests.

## Chapter 12 Summary

Google Mock isolates units from heavy dependencies. Property-based testing explores large input spaces. Systematic edge-case tests close gaps that example-based tests often leave. Use each technique where it clearly reduces risk.

## Questions and Exercises

1. When is a mock preferable to a simple fake?
2. Write a Google Mock test for a class that depends on an `authorize` method.
3. State one property that should hold for a pure `clamp` function.
4. List three edge values for a function that accepts a `std::vector<int>`.
5. True or false: Property-based testing eliminates the need for ordinary example-based unit tests. Explain.
