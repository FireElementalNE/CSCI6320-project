#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <regex>
#include "account.h"
#include "server.h"
#include "bank.h"
#include "constants.h"
#include "gen_functions.h"
using namespace std;
Bank::Bank(string accounts_dir) {
	init_accounts(accounts_dir);
	cout << "Bank loaded " << bank_accounts.size() << " accounts." << endl;
}
bool Bank::is_an_account(string an) {
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].chk_account(an)) {
			return true;
		}
	}
	return false;
}
bool Bank::lookup_account(string an, int p) {
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].chk_account(an)) {
			if(bank_accounts[i].check_creds(an, p)) {
				return true;
			}
		}
	}
	return false;
}

bool Bank::deposit(string an, int p, int amount) {
	if(!lookup_account(an, p)) {
		return false;
	}
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].chk_account(an)) {
			return bank_accounts[i].deposit(amount);
		}
	}
	cerr << "deposit: I shouldnt get here" << endl;
	return false;
}
int Bank::balance_inq(string an, int p) {
	if(!lookup_account(an, p)) {
		return -1;
	}
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].chk_account(an)) {
			return bank_accounts[i].get_balance(an, p);
		}
	}
	cerr << "balance_inq: I shouldnt get here" << endl;
	return -1;
}
bool Bank::withdraw(string an, int p, int amount) {
	if(!lookup_account(an, p)) {
		return false;
	}
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].chk_account(an)) {
			return bank_accounts[i].withdraw(an, p, amount);
		}
	}
	cerr << "withdraw: I shouldnt get here" << endl;
	return false;
}
bool Bank::transfer(string an, int p, string t_account, int amount) {
	if(an == t_account) {
		cerr << "accounts are the same" << endl;
		return false;
	}
	if(!lookup_account(an, p)) {
		return false;
	}
	if(!is_an_account(t_account)) {
		return false;
	}
	bool withdraw_success = false;
	bool deposit_success = false;
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].chk_account(an)) {
			withdraw_success = bank_accounts[i].withdraw(an, p, amount);
		}
	}
	if(!withdraw_success) {
		return false;
	}
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].chk_account(t_account)) {
			bank_accounts[i].load_account();
			deposit_success = bank_accounts[i].deposit(amount);
			if(!deposit_success) {
				cerr << "could not transfer money, returning it ot original account." << endl;
				bank_accounts[i].save_account();
				for(unsigned int i = 0; i < bank_accounts.size(); i++) {
					if(bank_accounts[i].chk_account(an)) {
						deposit_success = bank_accounts[i].deposit(amount);
					}
				}
				if(!deposit_success) {
					cerr << "ERROR! ERROR! account " << an << " is owed " << amount << "." << endl;
					return false;
				}
				return false;
			}
			bank_accounts[i].save_account();
		}
	}
	return true;

}
void Bank::init_accounts(string accounts_dir) {
	vector <string> account_names = get_account_names(accounts_dir);
	for(unsigned int i = 0; i < account_names.size(); i++) {
		account act = account(account_names[i], accounts_dir);
		bank_accounts.push_back(act);
	}
}
bool Bank::lock_act(string an, int p) {
	if(!is_an_account(an)) {
		cerr << "account " << an << " does not exist." << endl;
	}
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].chk_account(an)) {
			if(bank_accounts[i].is_locked()) {
				cerr << "bank account locked." << endl;
				return false;
			}
			bool check = bank_accounts[i].load_account();
			if(!bank_accounts[i].check_creds(an, p)) {
				cerr << "Invalid creds." << endl;
				bank_accounts[i].save_account();
				return false;
			}
			else if(!check) {
				cerr << "Error loading account." << endl;
				return false;	
			}
			else {
				cout << "succesfully loaded account" << endl;
				return true;
			}
		}
	}
	cerr << "could not find account." << endl;
	return false;
}
bool Bank::unlock_act(string an, int p) {
	for(unsigned int i = 0; i < bank_accounts.size(); i++) {
		if(bank_accounts[i].chk_account(an)) {
			if(!bank_accounts[i].is_locked()) {
				cerr << "bank account isnt locked." << endl;
				return false;
			}
			if(!bank_accounts[i].check_creds(an, p)) {
				cerr << "Invalid creds." << endl;
				return false;
			}
			else {
				cout << "succesfully saved account." << endl;
				bank_accounts[i].save_account();
				return true;
			}
		}
	}
	cerr << "could not find account." << endl;
	return false;
}
