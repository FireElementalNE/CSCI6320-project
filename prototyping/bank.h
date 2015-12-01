#ifndef BANK_H
#define BANK_H
#include <iostream>
#include <vector>
#include "account.h"
using namespace std;
class Bank {
private:
	string filename;
	vector <account> bank_accounts;
	void init_accounts(string fname);
public:
	Bank() {}
	Bank(string fname);
	bool is_an_account(int an);
	void load_bank();
	bool lookup_account(int an, int p);
	bool deposit(int an, int p, int amount);
	int balance_inq(int an, int p);
	bool withdraw(int an, int p, int amount);
	bool transfer(int an, int p, int t_account, int amount);
	void write_accounts();
};
#endif /* BANK_H */