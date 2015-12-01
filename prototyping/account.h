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
	string get_an();
	bool load_account();
	bool save_account();
	bool check_creds(string an, int p);
	bool deposit(int amount);
	bool withdraw(string an, int p, int amount);
	int get_balance(string an, int pin);
	bool is_locked();
	bool check_file();
	bool transfer(string an, int p, int amount);
	bool chk_account(string an);
};
#endif /* ACCOUNT_H */