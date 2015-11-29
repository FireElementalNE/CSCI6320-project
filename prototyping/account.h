#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <iostream>
using namespace std;
class account {
private:
	int account_num;
	char * pin_hash;
	int balance;
	bool logged_in;
	bool locked;
	bool check_account_num();
	bool check_pin(int i);
public:
	account(int an, char * p, int b, bool lock);
	bool check_creds(int an, char * p_hash);
	bool check_creds_act(account act);
	bool deposit(int amount);
	bool withdraw(int amount);
	int get_balance();
	bool is_locked();
	void lock();
	void login();
	void logout();
	bool is_logged_in();
};
#endif /* ACCOUNT_H */