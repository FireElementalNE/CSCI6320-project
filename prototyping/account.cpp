#include <iostream>
#include <string.h>
#include <fstream>
#include "crypto.h"
#include "account.h"
#include "gen_functions.h"
using namespace std;
account::account(string an, string accounts_dir) {
	account_filename = accounts_dir + "/" + an + ".card";
	cout << account_filename << endl;
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
	cout << pin << " " << balance << endl;
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
bool account::withdraw(int amount) {
	if(!is_locked()) {
		cerr << "Withdraw unsuccessful account not locked." << endl;
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
		cerr << "Deposit unsuccessful account not locked." << endl;
		return false;
	}
	balance += amount;
	return true;
}
int account::get_balance() {
	if(!is_locked()) {
		cerr << "Balance Inq unsuccessful account not locked." << endl;
		return -1;
	}
	return balance;
}
bool account::check_creds_act(account act) {
	if(!is_locked()) {
		cerr << "check_creds_act unsuccessful account not locked." << endl;
		return false;
	}
	if(act.check_creds(account_name, pin)) {
		return true;
	}
	return false;
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