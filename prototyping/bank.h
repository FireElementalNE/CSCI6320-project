#ifndef BANK_H
#define BANK_H
#include <iostream>
#include <vector>
#include "account.h"
using namespace std;
class Bank {
private:
	vector <account> bank_accounts;
	void init_accounts(string filename);
public:
	Bank() {}
	Bank(string filename);
	bool lookup_account(int an, int p);
	bool deposit(int an, int p, int amount);
	int balance_inq(int an, int p);
	bool withdraw(int an, int p, int amount);
};
#endif /* BANK_H */