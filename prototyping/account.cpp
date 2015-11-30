#include <iostream>
#include <string.h>
#include "crypto.h"
#include "account.h"
#include "gen_functions.h"
#include <openssl/sha.h>
using namespace std;
account::account(int an, char * p, int b, bool lock) {
	account_num = an;
	pin = atoi(p);
	balance = b;
	locked = lock;
}
bool account::check_creds(int an, int p) {
	if(account_num != an) {
		/*cout << "account mismatch" << endl;
		cout << "\t\'" << account_num << "\'" << endl;
		cout << "\t\'" << an << "\'" << endl;*/
		return false;
	}
	if(pin != p) {
		/*cout << "pin mismatch" << endl;
		cout << "\t" << pin << endl;
		cout << "\t" << p << endl;*/
		return false;
	}
	return true;
}
bool account::withdraw(int amount) {
	// client (ATM) muse interpret these codes...
	if(amount > balance) {
		return false;
	}
	else {
		balance = balance - amount;
		return true;
	}
}
bool account::deposit(int amount) {
	balance += amount;
	return true;
}
int account::get_balance() {
	return balance;
}
bool account::check_creds_act(account act) {
	if(act.check_creds(account_num, pin)) {
		return true;
	}
	return false;
}
bool account::is_locked() {
	return locked;
}
void account::lock() {
	locked = true;
}
void account::login() {
	logged_in = true;
}
void account::logout() {
	logged_in = false;
}
bool account::is_logged_in() {
	return logged_in;
}
