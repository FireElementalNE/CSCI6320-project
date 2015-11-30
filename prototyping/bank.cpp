#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "server.h"
#include "bank.h"
#include "constants.h"
using namespace std;
Bank::Bank(string filename) {
	init_accounts(filename);
	cout << "Bank loaded " << bank_accounts.size() << " accounts." << endl;
}
bool Bank::lookup_account(int an, int p) {
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].check_creds(an, p)) {
			cout << "Account Found!" << endl;
			return true;
		}
	}
	cerr << "Account not found." << endl;
	return false;
}

bool Bank::deposit(int an, int p, int amount) {
	if(!lookup_account(an, p)) {
		return false;
	}
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].check_creds(an, p)) {
			if(bank_accounts[i].is_locked()) {
				cerr << "deposit: Bank account locked." << endl;
				return false;
			}
			else {
				bank_accounts[i].deposit(amount);
				return true;
			}
		}
	}
	cerr << "deposit: I shouldnt get here" << endl;
	return false;
}
int Bank::balance_inq(int an, int p) {
	if(!lookup_account(an, p)) {
		return -1;
	}
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].check_creds(an, p)) {
			if(bank_accounts[i].is_locked()) {
				cerr << "balance_inq: Bank account locked." << endl;
			}
			else {
				return bank_accounts[i].get_balance();
			}
		}
	}
	cerr << "balance_inq: I shouldnt get here" << endl;
	return -1;
}
bool Bank::withdraw(int an, int p, int amount) {
	if(!lookup_account(an, p)) {
		return false;
	}
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].check_creds(an, p)) {
			if(bank_accounts[i].is_locked()) {
				cerr << "balance_inq: Bank account locked." << endl;
				return false;
			}
			else {
				bank_accounts[i].withdraw(amount);
				return true;
			}
		}
	}
	cerr << "withdraw: I shouldnt get here" << endl;
	return false;
}
void Bank::init_accounts(string filename) {
	ifstream file;
	file.open(filename);
	if(!file.is_open()) {
		cerr << "Could not open file \'" << filename << "\'." << endl;
		exit(EXIT_FAILURE);
	}
	string act_str, pin, balance, locked;
	while(file >> act_str >> pin >> balance >> locked) {
		int an = atoi(act_str.c_str());
		char * p = new char[MAX_PIN_SIZE];
		strncpy(p, pin.c_str(), MAX_PIN_SIZE);
		int b = atoi(balance.c_str());
		int l = (locked == "0") ? false : true;
		account act = account(an, p, b, l);
		bank_accounts.push_back(act);
	}
}