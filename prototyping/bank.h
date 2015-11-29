#ifndef BANK_H
#define BANK_H
#include <iostream>
#include <vector>
#include "account.h"
using namespace std;
class Bank {
private:
	vector <account> bank_accounts;
public:
	Bank() {}
	Bank(string filename);
};
#endif /* BANK_H */