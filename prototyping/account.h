#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <iostream>
using namespace std;
class account {
private:
	int account_num;
	char * pin_hash;
	int balance;
	bool is_logged_in;
	bool check_account_num();
	bool check_pin(int i);
public:
	account(int an, int p, int b);
	bool check_creds(int an, int p);
	bool deposit(int amount);
	bool withdraw(int amount);
	int get_balance();
};
#endif /* ACCOUNT_H */