#include <iostream>
#include <string.h>
#include "crypto.h"
#include "account.h"
#include <openssl/sha.h>
using namespace std;
account::account(int an, int p, int b) {
	account_num = an;
	pin_hash = new char[SHA_DIGEST_LENGTH];
	pin_hash = hash_pin(p);
	balance = b;
}
bool account::check_creds(int an, int p) {
	if(account_num != an) {
		return false;
	}
	char * p_hash = new char[SHA_DIGEST_LENGTH];
	p_hash = hash_pin(p);
	if(strcmp(p_hash,pin_hash) != 0) {
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