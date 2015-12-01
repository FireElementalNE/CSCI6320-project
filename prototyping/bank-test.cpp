#include <iostream>
#include "bank.h"
int main() {
	Bank bank = Bank("./accounts");
	bank.lock_act("alice", 1);
	cout << bank.withdraw("alice", 1, 5) << endl;
	cout << bank.balance_inq("alice", 1) << endl;
	cout << bank.deposit("alice", 1, 100) << endl;
	cout << bank.balance_inq("alice", 1) << endl;
	cout << bank.unlock_act("alice", 1) << endl;
	// cout << bank.transfer("alice", 1, "bob") << endl;
	return 0;
}