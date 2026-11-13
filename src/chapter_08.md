# Chapter 8: Test Organization and Maintenance

A test suite that cannot be navigated or safely changed will be neglected. Good organization—clear names, logical grouping, fixtures for shared setup, and disciplined maintenance—keeps the suite an asset rather than a liability.

## 8.1 Structuring Test Suites and Test Cases

Choose names that read as specifications:

- Suite name: the class or component under test (`MathOperationsTest`, `BankAccountTest`).
- Case name: the scenario or property (`AddsTwoPositiveIntegers`, `RejectsNegativeDeposit`).

Group related cases in the same suite. Keep each test function focused on one behavior. Place tests in files that mirror the production layout.

## 8.2 Grouping Related Tests Using Test Fixtures

When several tests need the same objects or cleanup, derive a fixture from `::testing::Test` and use `TEST_F`.

```cpp
#include <gtest/gtest.h>
#include "bank_account.h"
#include <memory>

class BankAccountTest : public ::testing::Test {
 protected:
  void SetUp() override {
    account_ = std::make_unique<BankAccount>(100);
  }
  void TearDown() override {
    account_.reset();
  }
  std::unique_ptr<BankAccount> account_;
};

TEST_F(BankAccountTest, DepositIncreasesBalance) {
  account_->deposit(50);
  EXPECT_EQ(account_->balance(), 150);
}

TEST_F(BankAccountTest, StartsWithInitialBalance) {
  EXPECT_EQ(account_->balance(), 100);
}
```

Each test receives a fresh account. For expensive shared state, override the static `SetUpTestSuite()` / `TearDownTestSuite()` methods.

## 8.3 Maintaining Tests as Code Evolves

- Update or delete tests in the same commit that changes the corresponding behavior.
- Keep tests compiling and passing on the main branch.
- Prefer adding a new test over weakening an existing assertion.
- Run the relevant subset locally (`--gtest_filter=BankAccountTest.*`) before pushing.
- Eliminate shared mutable state to avoid order-dependent failures.

## 8.4 Refactoring Tests for Better Organization

Typical progression:

1. Several free `TEST` macros share identical setup → move to a fixture.
2. A single test file becomes too long → split by component.
3. Parameter lists become unwieldy → extract named constants.
4. Repeated assertion patterns → small helper functions that still call `EXPECT_*`.

## 8.5 Case Study: Organizing and Maintaining a Growing Test Suite

Start with the Chapter 1 math tests. After adding more operations and edge cases, refactor into focused files or clear suite names, extract magic numbers, and ensure `--gtest_filter` can select meaningful subsets.

## Common Pitfalls

- Tests that depend on execution order.
- Fixtures that perform unnecessary work.
- Overly broad suite names that defeat filtering.
- Leaving disabled or commented-out tests indefinitely.
- Copy-paste tests that diverge silently.

## Chapter 8 Summary

Name suites and cases so they document behavior, group related tests, and use fixtures for shared setup and guaranteed cleanup. Maintain tests in lock-step with production changes and eliminate shared mutable state.

## Questions and Exercises

1. What problem does a test fixture solve that copying setup into every test does not?
2. Write a fixture for a simple `Stack` class that pushes two elements in `SetUp`.
3. Why should `TearDown` release resources even when a test fails?
4. A test passes alone but fails in the full suite. What is the most likely cause?
5. True or false: `SetUpTestSuite` runs before every individual test case. Explain.
