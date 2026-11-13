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

TEST(SafeSubstrTest, SuccessfulCaseUsesExpect) {
  const std::string input = "hello";
  std::string result = safe_substr(input, 1, 3);
  EXPECT_EQ(result, "ell");
  EXPECT_EQ(result.size(), 3u);
}

// Pointer precondition example
int* maybe_allocate(bool succeed) {
  return succeed ? new int(42) : nullptr;
}

TEST(PointerTest, AssertProtectsDereference) {
  int* p = maybe_allocate(false);
  ASSERT_NE(p, nullptr) << "allocation failed";
  EXPECT_EQ(*p, 42);
  delete p;
}
