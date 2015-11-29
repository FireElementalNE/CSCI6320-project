#include <iostream>
#include <string.h>
#include "crypto.h"
#include "account.h"
#include "gen_functions.h"
#include <openssl/sha.h>
using namespace std;
account::account(int an, char * p, int b, bool lock) {
	account_num = an;
	pin_hash = new char[SHA_DIGEST_LENGTH];
	pin_hash = hash_pin(p);
	balance = b;
	locked = lock;
	cout << account_num << " " << raw_to_hex((unsigned char*)pin_hash, SHA_DIGEST_LENGTH) << endl;
}
bool account::check_creds(int an, char * p_hash) {
	if(account_num != an) {
		cout << "account mismatch" << endl;
		cout << "\t\'" << account_num << "\'" << endl;
		cout << "\t\'" << an << "\'" << endl;
		return false;
	}
	if(strcmp(p_hash,pin_hash) != 0) {
		cout << "pin mismatch" << endl;
		cout << "\t\'" << raw_to_hex((unsigned char*)pin_hash, SHA_DIGEST_LENGTH) << "\'" << endl;
		cout << "\t\'" << raw_to_hex((unsigned char*)p_hash, SHA_DIGEST_LENGTH) << "\'" << endl;
		cout << strcmp(p_hash,pin_hash) << endl;
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
	if(act.check_creds(account_num, pin_hash)) {
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