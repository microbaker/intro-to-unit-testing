#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

class BankAccount {
 public:
  explicit BankAccount(int initial) : balance_(initial) {}
  void deposit(int amount) { balance_ += amount; }
  int balance() const { return balance_; }
 private:
  int balance_;
};

#endif
