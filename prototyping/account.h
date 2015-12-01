#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <iostream>
using namespace std;
class account {
private:
	string account_name;
	string account_filename;
	string lock_filename;
	int pin;
	int balance;
	void reset_vars();
	string account_str();
public:
	account(string an, string accounts_dir);
	bool load_account();
	bool save_account();
	bool check_creds(string an, int p);
	bool check_creds_act(account act);
	bool deposit(int amount);
	bool withdraw(int amount);
	int get_balance();
	bool is_locked();
	bool check_file();
	bool chk_account(string an);
};
#endif /* ACCOUNT_H */