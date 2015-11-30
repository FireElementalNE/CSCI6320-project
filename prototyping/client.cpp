#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "client.h"
#include "gen_functions.h"
#include "crypto.h"
#include "constants.h"
using namespace std;
CryptoClient::CryptoClient(int p1, string h1, bool d1, string filename_pub, string filename_priv) {
	port = p1;
	host = h1;
	debug = d1;
	pub_key = read_keyfile(filename_pub);
  	priv_key = read_keyfile(filename_priv);
	server_pub_key = "BLANK";
}
bool CryptoClient::init_connection() {
	if(debug) {
		cout << "DEBUG: connecting to " << host << " on port " << port << endl;
	}
	hostEntry = gethostbyname(host.c_str());
	if (!hostEntry) {
		cerr << "No such host name: " << host << endl;
		return false;
	}
	memset(&server_addr,0 , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
  	// create socket
	server = socket(PF_INET,SOCK_STREAM,0);
	if (server < 0) {
		perror("socket");
		return false;
	}
	int connect_rc = connect(server,(const struct sockaddr *)&server_addr,sizeof(server_addr));
 	// connect to server
	if (connect_rc < 0) {
		perror("connect");
		return false;
	}
	get_public_key();

	return true;
}
string CryptoClient::send_recv_msg(string msg) {
	char * buf = new char[BUF_LEN + 1];
	if(msg.size() > 0) {
	    // write the data to the server
	    char * new_msg = new char[msg.size()];
	    char * enc_msg = new char[ENC_LEN];
	    strncpy(new_msg, msg.c_str(), msg.size());
	    enc_msg = encr_msg((unsigned char *) new_msg, msg.size(), server_pub_key);
		send(server, enc_msg, BUF_LEN, 0);
		if(debug) {
			string hex_tmp = raw_to_hex((unsigned char *)enc_msg, ENC_LEN);
			cout << "DEBUG: sent '" << hex_tmp << "' to server." << endl;
		}
	    // read the response
		memset(buf,0,BUF_LEN);
		recv(server, buf, BUF_LEN, 0);
		string received = decr_msg((unsigned char*) buf, priv_key);
		if(received == "") {
			cerr << "ERROR: decryption failed. Sorry." << endl;
		}
	    // print the response
		if(debug) {
			string hex_tmp = raw_to_hex((unsigned char *)buf, ENC_LEN);
			cout << "DEBUG: got '" << hex_tmp << "' from server." << endl;
		}
		return received;
	}
	else {
		cerr << "ERROR: empty line." << endl;
		return "";
	}
}

void CryptoClient::get_public_key() {
	char * buf = new char[BUF_LEN];
	string msg = "PUBKEY";
	send(server, msg.c_str(), msg.length(), 0);
	// this might work (using pub_key.size() instead of BUF_LEN)
	recv(server, buf, BUF_LEN, 0);
	server_pub_key = (string)buf;
	cout << "Secure Connection Established." << endl;
	send(server, pub_key.c_str(), pub_key.size(), 0);
	recv(server, buf, ENC_LEN, 0);
	string received = decr_msg((unsigned char*) buf, priv_key);
	if(received == "OK.") {
		string pin_str, act_str;
		cout << "Enter Account #: ";
		cin >> act_str;
		cout << "Enter Pin: ";
		cin >> pin_str;
		char * act_char = new char[MAX_ACT_SIZE];
		char * pin_char = new char[MAX_PIN_SIZE];
		char * login_info = new char[MAX_ACT_SIZE + SHA_DIGEST_LENGTH];
		strncpy(act_char, act_str.c_str(), MAX_ACT_SIZE);
		strncpy(pin_char, pin_str.c_str(), MAX_PIN_SIZE);
		strcat(login_info, act_char);
		strcat(login_info, ":");
		strcat(login_info, pin_char);
		// char * act_enc = new char[ENC_LEN];
		// char * pin_enc = new char[ENC_LEN];
		// act_enc = encr_msg((unsigned char*)act_char, act_str.size(), server_pub_key);
		// pin_enc = encr_msg((unsigned char*)pin_char, SHA_DIGEST_LENGTH, server_pub_key);
		// send(server, act_enc, ENC_LEN, 0);
		cout << login_info << endl;
		char * login_info_enc = new char[ENC_LEN];
		login_info_enc = encr_msg((unsigned char *)login_info, act_str.size() + SHA_DIGEST_LENGTH, server_pub_key);
		send(server, login_info_enc, ENC_LEN, 0);
		recv(server, buf, ENC_LEN, 0);
		string received = decr_msg((unsigned char*) buf, priv_key);
		char * command_enc;
		if(received != "FAILURE."){
			string command = "";
			do {
				command_enc = new char[ENC_LEN];
				cout << "Welcome " << received << "!" << endl;
				cout << "Main Menu: " << endl;
				cout << "[1] Withdraw" << endl;
				cout << "[2] balance" << endl;
				cout << "[3] logout" << endl;
				cout << ">";
				cin >> command;
				int command_int = atoi(command.c_str());
				if(command_int != 1 && command_int != 2 && command_int != 3) {
					cerr << "Invalid choice." << endl;
				}
				else {
					if(command_int == 1) {
						char * server_resp = new char[ENC_LEN];
						// for buffer overflow
						char * payload_safe = new char[MAX_AMT_LEN];
						string amount_str;
						cout << "Amount>";
						cin >> amount_str;
						string payload = "W:" + amount_str;
						strncpy(payload_safe, payload.c_str(), MAX_AMT_LEN + 2);
						command_enc = encr_msg((unsigned char *)payload_safe, strlen(payload_safe), server_pub_key);
						send(server, command_enc, ENC_LEN, 0);
						recv(server, server_resp, ENC_LEN, 0);
						string response = decr_msg((unsigned char*)server_resp, priv_key);
						cout << "server: " << response << endl;
					}
					
				}

			}
			while(command != "exit");
			char * exit_command = new char[5];
			char * exit_command_enc = new char[ENC_LEN];
			strncpy(exit_command, "EXIT.", 5);
			exit_command_enc = encr_msg((unsigned char *)exit_command, 5, server_pub_key);
			send(server, exit_command_enc, ENC_LEN, 0);
		}
	}
}
void CryptoClient::close_connection() {
	close(server);
}