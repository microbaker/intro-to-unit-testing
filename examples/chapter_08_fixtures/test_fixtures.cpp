#include <gtest/gtest.h>
#include "bank_account.h"
#include <memory>

class BankAccountTest : public ::testing::Test {
 protected:
  void SetUp() override {
    account_ = std::make_unique<BankAccount>(100);
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

TEST_F(BankAccountTest, MultipleDeposits) {
  account_->deposit(20);
  account_->deposit(30);
  EXPECT_EQ(account_->balance(), 150);
}
