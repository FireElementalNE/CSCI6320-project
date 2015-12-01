#ifndef BANK_H
#define BANK_H
#include <iostream>
#include <vector>
#include "account.h"
using namespace std;
class Bank {
private:
	vector <account> bank_accounts;
	void init_accounts(string accounts_dir);
public:
	Bank() {}
	Bank(string accounts_dir);
	bool is_an_account(string an);
	void load_bank();
	bool lookup_account(string an, int p);
	bool deposit(string an, int p, int amount);
	int balance_inq(string an, int p);
	bool withdraw(string an, int p, int amount);
	bool transfer(string an, int p, string t_account, int amount);
	bool lock_act(string an, int p);
	bool unlock_act(string an, int p);
};
#endif /* BANK_H */