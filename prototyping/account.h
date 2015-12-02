#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <iostream>
class account {
private:
	std::string account_name;
	std::string account_filename;
	std::string lock_filename;
	int pin;
	int balance;
	void reset_vars();
	std::string account_str();
	void save_changes();
	void read_changes();
public:
	account(std::string an, std::string accounts_dir);
	std::string get_an();
	bool load_account();
	bool save_account();
	bool check_creds(std::string an, int p);
	bool deposit(int amount);
	bool withdraw(std::string an, int p, int amount);
	int get_balance(std::string an, int pin);
	bool is_locked();
	bool check_file();
	bool chk_account(std::string an);

};
#endif /* ACCOUNT_H */