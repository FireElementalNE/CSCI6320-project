#include <iostream>
#include "client.h"
using namespace std;
void print_usage(char * argv0);
int main(int argc, char ** argv) {
	int c;
	int port = 5555;
	string host = "localhost";
	bool debug = false;
	while ((c = getopt(argc, argv, "dhp:T:")) != -1) {
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
		}
	}
	CryptoClient client = CryptoClient(port, host, debug);
	bool test = client.init_connection();
	cout << "got here" << endl;
	if(!test) {
		cerr << "what?" << endl;
	}
	// allocate buffer
  	// read a line from standard input
	string line;
	while (getline(cin,line)) {
		string resp = client.send_recv_msg(line);
		cout << "GOT: " << resp << endl;;
	}
	client.close_connection();
}
void print_usage(char * argv0) {
	cout << "usage: " << argv0 << " [-d] [-h] [-p port] [-T hostname] -m msg" << endl;
	cout << "\t-h help (show this menu)" << endl;
	cout << "\t-d Debug flag" << endl;
	cout << "\t-T hostname to connect to (DEFAULT: 'localhost')" << endl;
	cout << "\t-p port to connect to (DEFAULT: 5555)" << endl;
}