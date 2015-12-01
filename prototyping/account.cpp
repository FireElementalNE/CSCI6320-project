#include <iostream>
#include <string.h>
#include <fstream>
#include "crypto.h"
#include "account.h"
#include "gen_functions.h"
using namespace std;
string account::get_an() {
	return account_name;
}
account::account(string an, string accounts_dir) {
	account_filename = accounts_dir + "/" + an + ".act";
	lock_filename = accounts_dir + "/" + an + ".lock";
	account_name = an;
	reset_vars();
}
bool account::load_account() {
	if(is_locked()) {
		cerr << "Cannot Load account, locked" << endl;
		return false;
	}
	if(!check_file()) {
		return false;
	}
	ifstream file;
	file.open(account_filename);
	if(!file.is_open()) {
		cerr << "Could not open file " << account_filename << "." << endl;
		return false;
	}
	string pin_str, balance_str;
	file >> pin_str >> balance_str;
	pin = atoi(pin_str.c_str());
	balance = atoi(balance_str.c_str());
	fstream fs;
    fs.open(lock_filename, ios::out);
    fs.close();
    file.close();
	return true;
}
bool account::check_file() {
	if(!file_exists(account_filename)) {
		cerr << "File " << account_filename << " does not exist." << endl;
		return false;
	}
	return true;
}
bool account::save_account() {
	if(!is_locked()) {
		cerr << "Cannot save account, not locked" << endl;
		return false;
	}
	remove(lock_filename.c_str());
	remove(account_filename.c_str());
	ofstream file;
	file.open(account_filename);
	if(!file.is_open()) {
		cerr << "this is bad..." << endl;
		exit(EXIT_FAILURE);
	}
	file << account_str() << endl;
	file.close();
	reset_vars();
	return true;
}
void account::reset_vars() {
	balance = -1;
	pin = -1;
}
bool account::check_creds(string an, int p) {
	if(!is_locked()) {
		cerr << "check_creds unsuccessful account not locked." << endl;
		return false;
	}
	if(account_name != an) {
		return false;
	}
	if(pin != p) {
		return false;
	}
	return true;
}
bool account::transfer(string an, int p, int amount) {
	if(is_locked()) {
		return false;
	}
	load_account();
	if(!check_creds(an, p)) {
		cerr << "transfer: bad credentials." << endl;
		return false;
	}
	deposit(amount);
	save_account();
	return true;
}
bool account::withdraw(string an, int p, int amount) {
	if(!is_locked()) {
		cerr << "withdraw: unsuccessful account not locked." << endl;
		return false;
	}
	if(!check_creds(an, p)) {
		cerr << "withdraw: bad credentials." << endl;
		return false;
	}
	if(amount > balance) {
		return false;
	}
	else {
		balance = balance - amount;
		return true;
	}
}
bool account::deposit(int amount) {
	if(!is_locked()) {
		cerr << "deposit: unsuccessful account not locked." << endl;
		return false;
	}
	balance += amount;
	return true;
}
int account::get_balance(string an, int p) {
	if(!is_locked()) {
		cerr << "get_balance: unsuccessful account not locked." << endl;
		return -1;
	}
	if(!check_creds(an, p)) {
		cerr << "get_balance: bad credentials." << endl;
		return false;
	}
	return balance;
}
bool account::is_locked() {
	return file_exists(lock_filename);
}
bool account::chk_account(string an) {
	return an == account_name;
}
string account::account_str() {
	return to_string(pin) + " " + to_string(balance);
}