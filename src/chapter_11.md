# Chapter 11: Best Practices for Unit Testing

Effective unit testing is as much about the shape of the production code and the habits of the team as it is about assertion macros. This chapter collects the practices that make tests easy to write, fast to run, and reliable over time.

## 11.1 Writing Testable Code from the Outset

Code is testable when its behavior can be exercised and observed without heroic effort. Characteristics that help include:

- Small functions and classes with a single responsibility.
- Dependencies received via constructor or method parameters (dependency injection).
- Minimal hidden state and side effects.
- Clear inputs and outputs.
- Interfaces or abstract bases at module boundaries.

**Less testable:**

```cpp
class ReportGenerator {
 public:
  void generate() {
    auto data = database_.query("SELECT …");
    // complex logic
    file_.write(result);
  }
 private:
  Database database_;
  File file_;
};
```

**More testable:**

```cpp
class ReportGenerator {
 public:
  ReportGenerator(DataSource& source, ResultSink& sink)
      : source_(source), sink_(sink) {}
  void generate() {
    auto data = source_.query("SELECT …");
    // same logic
    sink_.write(result);
  }
 private:
  DataSource& source_;
  ResultSink& sink_;
};
```

## 11.2 Following the FIRST Principles

- **Fast.** Tests should run in milliseconds. Avoid real network, disk, or database access.
- **Independent.** No test should depend on another test’s side effects or on execution order.
- **Repeatable.** The same inputs always produce the same pass/fail result.
- **Self-validating.** The test itself decides pass or fail via assertions.
- **Timely.** Write the test close to the code—ideally just before or immediately after the production change.

Google Test supports these principles directly through isolation, fixtures, deterministic assertions, and fast execution when external dependencies are avoided.

**Brief note on Test-Driven Development.** TDD cycles through “write a failing test, write the minimum code to pass, refactor.” It tends to produce testable designs because the test is written first.

## 11.3 Balancing Comprehensive Tests and Development Speed

- Prioritize public behavior and high-risk logic over trivial getters.
- Use parameterized tests to cover many values with one body.
- Keep the default local suite under a few seconds; move slower checks to a separate CI stage.
- Measure coverage to find untested branches, then add focused tests.
- Delete or rewrite tests that no longer document current intent.

## Common Pitfalls

- Hidden dependencies that force every test to stand up a heavy environment.
- Tests that pass only on the author’s machine.
- Suites so slow that developers run only a subset.
- Writing tests long after the code.
- Over-mocking internal details so that refactoring breaks tests even when behavior is unchanged.

## Chapter 11 Summary

Design production code so that dependencies are injectable and behavior is observable. Follow the FIRST principles so the suite stays fast, independent, repeatable, self-validating, and timely. Balance breadth of coverage against speed so that developers continue to run the tests.

## Questions and Exercises

1. List three concrete changes that make a class easier to unit test.
2. Rewrite the less-testable `ReportGenerator` sketch so that both collaborators are injected.
3. For each letter of FIRST, give one Google Test practice that supports it.
4. Why does a test that reads the system clock violate repeatability?
5. True or false: A suite that takes ten minutes to run still provides most of the benefit of unit testing. Explain.
6. What is one advantage of writing the test before the production code?
