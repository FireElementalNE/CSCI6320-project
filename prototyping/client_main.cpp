#include <iostream>
#include <unistd.h>
#include "client.h"
using namespace std;
void print_usage(char * argv0);
int main(int argc, char ** argv) {
	int c;
	int port = 5555;
	string host = "localhost";
	bool debug = false;
	string priv_key = "keys/private_client.pem";
	string pub_key = "keys/public_client.pem";
	string accounts_dir = "./accounts_atm/";
	while ((c = getopt(argc, argv, "dhp:T:a:")) != -1) {
		switch (c) {
		case 'h':
			print_usage(argv[0]);
			exit(EXIT_SUCCESS);
		case 'p':
			port = atoi(optarg);
			break;
		case 'd':
			debug = true;
			break;
		case 'T':
			host = optarg;
			break;
		case 'k':
			priv_key = optarg;
			break;
		case 'K':
			pub_key = optarg;
			break;
		case 'a':
			accounts_dir = optarg;
			break;
		}
	}
	CryptoClient client = CryptoClient(port, host, debug, pub_key, priv_key, accounts_dir);
	client.start_session();
	client.close_connection();
}
void print_usage(char * argv0) {
	cout << "usage: " << argv0 << " [-d] [-h] [-p port] [-T hostname] [-k private key] [-K public key] [-a accounts]" << endl;
	cout << "\t-h help (show this menu)" << endl;
	cout << "\t-d Debug flag" << endl;
	cout << "\t-T hostname to connect to (DEFAULT: 'localhost')" << endl;
	cout << "\t-p port to connect to (DEFAULT: 5555)" << endl;
	cout << "\t-a account directory for atm (DEFAULT: ./accounts_atm/" << endl;
}