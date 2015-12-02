#include <iostream>
#include <cstring>
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
		cerr << "account " << account_name << "-> cannot Load account, locked." << endl;
		return false;
	}
	if(!check_file()) {
		return false;
	}
	ifstream file;
	file.open(account_filename);
	if(!file.is_open()) {
		cerr << "account " << account_name << "-> could not open file " << account_filename << "." << endl;
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
		cerr << "account " << account_name << "-> file " << account_filename << " does not exist." << endl;
		return false;
	}
	return true;
}
bool account::save_account() {
	if(!is_locked()) {
		cerr << "account " << account_name << "-> cannot save account, not locked." << endl;
		return false;
	}
	remove(lock_filename.c_str());
	save_changes();
	reset_vars();
	return true;
}
void account::reset_vars() {
	balance = -1;
	pin = -1;
}
bool account::check_creds(string an, int p) {
	if(!is_locked()) {
		cerr << "account " << account_name << "-> check_creds: unsuccessful account not locked." << endl;
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
bool account::withdraw(string an, int p, int amount) {
	if(!is_locked()) {
		cerr << "account " << account_name << "-> withdraw: unsuccessful account not locked." << endl;
		return false;
	}
	if(!check_creds(an, p)) {
		cerr << "account " << account_name << "-> withdraw: bad credentials." << endl;
		return false;
	}
	if(amount > balance) {
		return false;
	}
	else {
		read_changes();
		balance = balance - amount;
		save_changes();
		return true;
	}
}
bool account::deposit(int amount) {
	if(!is_locked()) {
		cerr << "account " << account_name << "-> deposit: unsuccessful account not locked." << endl;
		return false;
	}
	read_changes();
	balance += amount;
	save_changes();
	return true;
}
int account::get_balance(string an, int p) {
	if(!is_locked()) {
		cerr << "account " << account_name << "-> get_balance: unsuccessful account not locked." << endl;
		return -1;
	}
	if(!check_creds(an, p)) {
		cerr << "account " << account_name << "-> get_balance: bad credentials." << endl;
		return false;
	}
	read_changes();
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
void account::save_changes() {
	if(!is_locked()) {
		cerr << "account " << account_name << "-> save_changes: account not locked." << endl;
		return;
	}
	remove(account_filename.c_str());
	ofstream file;
	file.open(account_filename);
	if(!file.is_open()) {
		cerr << "account " << account_name << "-> this is bad..." << endl;
		exit(EXIT_FAILURE);
	}
	file << account_str() << endl;
	file.close();
}
void account::read_changes() {
	if(!is_locked()) {
		cerr << "account " << account_name << "-> read_changes: account not locked." << endl;
		return;
	}
  	ifstream file_in;
	file_in.open(account_filename);
	if(!file_in.is_open()) {
		cerr << "could not open account." << endl;
		exit(EXIT_FAILURE);
	}
	string old_balance;
	string pin;
	file_in >> pin >> old_balance;
	balance = atoi(old_balance.c_str());
	file_in.close();
}