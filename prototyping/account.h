#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <iostream>
using namespace std;
class account {
private:
	int account_num;
	int pin;
	int balance;
	bool logged_in;
	bool locked;
public:
	account(int an, char * p, int b, bool lock);
	bool check_creds(int an, int p);
	bool check_creds_act(account act);
	bool deposit(int amount);
	bool withdraw(int amount);
	int get_balance();
	bool is_locked();
	void lock();
	void login();
	void logout();
	bool is_logged_in();
	bool chk_account(int an);
	string account_str();
};
#endif /* ACCOUNT_H */