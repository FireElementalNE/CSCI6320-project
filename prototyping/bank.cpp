#include <iostream>
#include <vector>
#include "account.h"
#include "server.h"
#include "bank.h"
bank::bank(string filename, int p1, string h1, bool d1, string filename_pub, string filename_priv) {
	server = CryptoServer(p1, h1, d1, filename_pub, filename_priv);
	server.start_server();
}