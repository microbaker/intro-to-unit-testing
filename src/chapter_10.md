# Chapter 10: Unit Testing and Software Security

Unit tests improve security mainly by locking down the logic that enforces security policies—input validation, bounds checking, authentication decisions, error handling that must not leak information, and correct use of safe APIs. They are far less effective at discovering memory-safety vulnerabilities, race conditions, or flaws that appear only under adversarial or environmental stress.

## 10.1 The Role Unit Testing Plays in Software Security

Well-chosen unit tests help security in several practical ways:

- They verify that validation and sanitization routines reject malformed or malicious inputs and accept well-formed ones.
- They confirm boundary behavior (empty strings, maximum lengths, extreme numeric values).
- They lock down error-handling paths so that failures do not leave the system in an insecure state or emit sensitive details.
- They document the expected secure behavior, making later regressions visible.

They do **not** reliably detect:

- Buffer overflows, use-after-free, or other memory-safety bugs.
- Injection flaws whose exploitability depends on a downstream interpreter or database.
- Timing side channels or configuration errors outside the unit.

For memory-safety issues, enable AddressSanitizer (and UndefinedBehaviorSanitizer) in at least one CI build. For input-driven crashes, add a fuzzer. Static analysis and dependency scanning catch additional classes of problems.

## 10.2 Generating Unit Tests That Identify Vulnerabilities and Reduce Risk

Focus unit tests on the decision points that affect security.

```cpp
#include <gtest/gtest.h>
#include <string>
#include <cctype>

bool is_valid_username(const std::string& name) {
  if (name.empty() || name.size() > 32) return false;
  for (char c : name) {
    if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') return false;
  }
  return true;
}

TEST(UsernameValidationTest, AcceptsWellFormed) {
  EXPECT_TRUE(is_valid_username("alice_01"));
  EXPECT_TRUE(is_valid_username("Bob"));
}

TEST(UsernameValidationTest, RejectsEmpty) {
  EXPECT_FALSE(is_valid_username(""));
}

TEST(UsernameValidationTest, RejectsTooLong) {
  EXPECT_FALSE(is_valid_username(std::string(33, 'a')));
}

TEST(UsernameValidationTest, RejectsInvalidCharacters) {
  EXPECT_FALSE(is_valid_username("alice;drop"));
  EXPECT_FALSE(is_valid_username("user name"));
  EXPECT_FALSE(is_valid_username("admin'--"));
}
```

A second example—ensuring an error path does not leak internal details—can assert on the exact error code or message category rather than on a raw exception string.

## 10.3 Comparing Secure and Insecure Code

**Insecure pattern:**

```cpp
void process_query(const std::string& user_input) {
  std::string sql = "SELECT * FROM users WHERE name = '" + user_input + "'";
  // execute sql … (vulnerable to injection)
}
```

**More secure pattern:**

```cpp
void process_query(const std::string& user_input) {
  if (!is_valid_username(user_input)) {
    throw std::invalid_argument("invalid username");
  }
  // use parameterized query / prepared statement
}
```

Memory-related example:

```cpp
std::optional<int> safe_at(const std::vector<int>& v, size_t i) {
  if (i >= v.size()) return std::nullopt;
  return v[i];
}

TEST(SafeAtTest, RejectsOutOfRange) {
  std::vector<int> v{10, 20};
  EXPECT_FALSE(safe_at(v, 2).has_value());
  EXPECT_EQ(safe_at(v, 1).value(), 20);
}
```

## Common Pitfalls

- Believing a passing unit test proves a function is free of buffer overflows or injection flaws.
- Writing tests that only exercise the happy path of security-critical checks.
- Asserting on exact exception text that contains internal details.
- Ignoring complementary tools that detect memory-safety defects.

## Chapter 10 Summary

Unit tests strengthen security by protecting validation, boundary, and error-handling logic. They are not sufficient for memory-safety, injection, or environmental vulnerabilities; pair them with sanitizers, fuzzing, static analysis, and proper use of safe APIs.

## Questions and Exercises

1. Name two security-relevant properties that unit tests can lock down effectively, and two classes of vulnerability they usually cannot detect.
2. Write unit tests for a function that must reject empty strings, strings containing `..`, and absolute paths.
3. Why is AddressSanitizer more effective than a Google Test assertion at finding a buffer overrun?
4. Can unit tests alone make a function that concatenates user input into a shell command safe?
5. True or false: High unit-test coverage of a module guarantees the absence of security vulnerabilities. Explain.
