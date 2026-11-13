# Chapter 7: Testing Private Code and Encapsulation

Private and protected members exist to protect invariants. Tests that reach past the public interface can lock those invariants in place or make future refactoring painful. The preferred approach is always to exercise private logic indirectly through public methods.

## 7.1 Techniques for Testing Private and Protected Methods

Three techniques are commonly used, listed from most to least preferable:

1. Test through the public interface.
2. Grant friendship to a specific test fixture (or use the `FRIEND_TEST` macro).
3. Create a test subclass that promotes protected members to public.

Avoid redefining `private` as `public` via macros.

## 7.2 Using Friend Classes to Access Private Members

```cpp
// include/bank_account.h
#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <gtest/gtest_prod.h>

class BankAccount {
 public:
  explicit BankAccount(int initial) : balance_(initial) {}
  void deposit(int amount) { balance_ += amount; }
  int balance() const { return balance_; }

 private:
  FRIEND_TEST(BankAccountTest, AppliesInterestCorrectly);
  void apply_interest(double rate) {
    balance_ = static_cast<int>(balance_ * (1.0 + rate));
  }
  int balance_;
};

#endif
```

```cpp
// test/test_bank_account.cpp
#include <gtest/gtest.h>
#include "bank_account.h"

TEST(BankAccountTest, AppliesInterestCorrectly) {
  BankAccount acct(1000);
  acct.apply_interest(0.05);
  EXPECT_EQ(acct.balance(), 1050);
}
```

## 7.3 Balancing Encapsulation and Testability

Prefer the public interface whenever the private logic can be observed through return values or state changes. Resort to friendship or a test subclass only when the private method contains complex logic that is awkward to reach indirectly, or when characterizing legacy code.

**Protected members via test subclass:**

```cpp
class Base {
 protected:
  int hidden_calculation(int x) const { return x * 2 + 1; }
 public:
  int public_wrapper(int x) const { return hidden_calculation(x); }
};

class TestableBase : public Base {
 public:
  using Base::hidden_calculation;
};

TEST(ProtectedTest, DirectAccess) {
  TestableBase tb;
  EXPECT_EQ(tb.hidden_calculation(10), 21);
}
```

## 7.4 Case Study: Testing Private Methods Without Sacrificing Encapsulation

```cpp
class EmailValidator {
 public:
  bool is_valid(const std::string& email) const {
    return !email.empty() && contains_at(email) && has_domain(email);
  }
 private:
  bool contains_at(const std::string& s) const {
    return s.find('@') != std::string::npos;
  }
  bool has_domain(const std::string& s) const {
    auto pos = s.find('@');
    return pos != std::string::npos && pos + 1 < s.size() &&
           s.find('.', pos) != std::string::npos;
  }
};
```

Preferred approach—test only the public method:

```cpp
TEST(EmailValidatorTest, AcceptsWellFormed) {
  EmailValidator v;
  EXPECT_TRUE(v.is_valid("user@example.com"));
}
TEST(EmailValidatorTest, RejectsMissingAt) {
  EmailValidator v;
  EXPECT_FALSE(v.is_valid("userexample.com"));
}
```

## Common Pitfalls

- Making every private method a friend of every test.
- Tests that fail after a pure refactoring of private implementation details.
- Using the `#define private public` trick.
- Forgetting that protected access via subclass still couples the test to the inheritance hierarchy.

## Chapter 7 Summary

Test private behavior through the public interface whenever possible. When direct access is truly required, grant friendship with `FRIEND_TEST` to a specific test or promote protected members in a test subclass. Keep such breaches narrow, documented, and temporary.

## Questions and Exercises

1. Why is testing exclusively through the public interface usually preferable?
2. Show the exact declaration that grants friendship to a test named `ParserTest.HandlesEmptyInput`.
3. When is a test subclass that promotes protected members appropriate?
4. True or false: The `FRIEND_TEST` macro makes the private members visible to all tests in the suite. Explain.
5. Identify two risks of the macro-based `#define private public` approach.
