#include <gtest/gtest.h>
#include <string>
#include <cctype>
#include <optional>
#include <vector>

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

TEST(UsernameValidationTest, RejectsEmptyAndTooLong) {
  EXPECT_FALSE(is_valid_username(""));
  EXPECT_FALSE(is_valid_username(std::string(33, 'a')));
}

TEST(UsernameValidationTest, RejectsInvalidCharacters) {
  EXPECT_FALSE(is_valid_username("alice;drop"));
  EXPECT_FALSE(is_valid_username("user name"));
  EXPECT_FALSE(is_valid_username("admin'--"));
}

std::optional<int> safe_at(const std::vector<int>& v, size_t i) {
  if (i >= v.size()) return std::nullopt;
  return v[i];
}

TEST(SafeAtTest, BoundsChecking) {
  std::vector<int> v{10, 20};
  EXPECT_FALSE(safe_at(v, 2).has_value());
  EXPECT_EQ(safe_at(v, 0).value(), 10);
  EXPECT_EQ(safe_at(v, 1).value(), 20);
}
