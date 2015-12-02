#include <iostream>
#include <unistd.h>
#include <cstring>
#include "client.h"
#include "gen_functions.h"
#include "constants.h"
using namespace std;
void print_usage(char * argv0);
int main(int argc, char ** argv) {
	if(argc < 2) {
		print_usage(argv[0]);
		exit(EXIT_SUCCESS);
	}
	char * port_safe = new char[MAX_PORT_LEN];
	memset(port_safe, '\0', MAX_PORT_LEN);
	strncpy(port_safe, argv[1], MAX_PORT_LEN * sizeof(char));
	int port = atoi(port_safe); 
	if(!check_port(port)) {
		cerr << "invalid port." << endl;
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	string host = "localhost";
	bool debug = false;
	string priv_key = "keys/private_client.pem";
	string pub_key = "keys/public_client.pem";
	string accounts_dir = "./accounts_atm/";
	string mac_key = "./client.mac_key";
	string mac_keys = "./mac_keys_client.trusted";
	int c;
	while ((c = getopt(argc, argv, "dhT:a:M:m:")) != -1) {
		switch (c) {
		case 'h':
			print_usage(argv[0]);
			exit(EXIT_SUCCESS);
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
		case 'M':
			mac_key = optarg;
			break;
		case 'm':
			mac_keys = optarg;
			break;
		}
	}
	CryptoClient client = CryptoClient(port, host, debug, pub_key, priv_key, accounts_dir, mac_key, mac_keys);
	client.start_session();
	client.close_connection();
}
void print_usage(char * argv0) {
	cout << "usage: " << argv0 << " <port> [-d] [-h] [-T hostname] [-k private key] [-K public key] [-a accounts] [-m mak_keys] [-M mac_keys]" << endl;
	cout << "\t port port to connect to (REQUIRED)" << endl;
	cout << "\t-h help (show this menu)" << endl;
	cout << "\t-d Debug flag" << endl;
	cout << "\t-T hostname to connect to (DEFAULT: 'localhost')" << endl;
	cout << "\t-a account directory for atm (DEFAULT: ./accounts_atm/" << endl;
	cout << "\t-M mac_key the mac key file (DEFAULT: client.mac_key)" << endl;
	cout << "\t-m mac_keys mac_keys the  trusted mac keys file (DEFAULT: mac_keys_client.trusted)" << endl;
}