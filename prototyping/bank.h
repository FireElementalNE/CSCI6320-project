#ifndef BANK_H
#define BANK_H
#include <iostream>
#include <vector>
#include "account.h"
#include "server.h"
using namespace std;
class bank {
private:
	vector <account> bank_accounts;
	CryptoServer server;
public:
	bank(string filename, int p1, string h1, bool d1, string filename_pub, string filename_priv);
};
#endif /* BANK_H */