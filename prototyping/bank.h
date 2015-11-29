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
	bool lookup_account(int an, char * p_hash);
	bool deposit(int an, char * p_hash, int amount);
	int balance_inq(int an, char * p_hash);
	bool withdraw(int an, char * p_hash, int amount);
};
#endif /* BANK_H */