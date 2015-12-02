#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include "client.h"
#include "gen_functions.h"
#include "crypto.h"
#include "constants.h"
using namespace std;
CryptoClient::CryptoClient(int p1, string h1, bool d1, string filename_pub, string filename_priv, string a_dir, string mf) {
	port = p1;
	host = h1;
	debug = d1;
	mac_filename = mf;
	pub_key = read_keyfile(filename_pub);
  	priv_key = read_keyfile(filename_priv);
  	accounts_dir = a_dir;
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
	return true;
}
void CryptoClient::start_session() {
	string command;
	string pin_str, act_str;
	do {
		fflush(stdout);
		regex login_regex(LOGIN_REGEX_STR);
		cout << "login> ";
		getline(cin, command);
		smatch login_match;
		regex_search(command, login_match, login_regex);
		if(login_match.size() == 2) {
			act_str = login_match[1];
		}
		else {
			continue;
		}
		string pin_str;
		cout << "PIN> ";
		fflush(stdout);
		getline(cin, pin_str);
		bool pin_test = check_pin(act_str, pin_str, accounts_dir);
		if(!pin_test) {
			cerr << "Incorrect PIN" << endl;
			continue;
		}
		bool test = init_connection();
		if(!test) {
			cerr << "Cannot connect to Bank." << endl;
			exit(EXIT_FAILURE);
		}
		char * buf = new char[BUF_LEN];
		string msg = "PUBKEY";
		send(server, msg.c_str(), msg.length(), 0);
		int nread = recv(server, buf, BUF_LEN, 0);
		if(nread <= 0) {
			cerr << "socket closed by server." << endl;
			close(server);
			continue;
		}
		server_pub_key = (string)buf;
		cout << "Secure Connection Established." << endl;
		send(server, pub_key.c_str(), pub_key.size(), 0);
		string my_mac = make_mac(mac_filename);
		send(server, my_mac.c_str(), my_mac.size(), 0);
		nread = recv(server, buf, ENC_LEN, 0);
		if(nread <= 0) {
			cerr << "socket closed by server." << endl;
			close(server);
			continue;
		}
		string received_init = decr_msg((unsigned char*) buf, priv_key);
		if(received_init == "NULL") {
			cerr << "could not decrypt received msg (reveived)" << endl;
			continue;
		}
		if(received_init == "OK.") {			
			string login_info;
			login_info = act_str + ":" + pin_str;
			char * login_info_enc = new char[ENC_LEN];
			login_info_enc = encr_msg_str(login_info, login_info.size(), server_pub_key);
			if(login_info_enc == NULL) {
				cerr << "could not encrypt login info. disconnecting." << endl;
				break;
			}
			send(server, login_info_enc, ENC_LEN, 0);
			nread = recv(server, buf, ENC_LEN, 0);
			if(nread <= 0) {
				cerr << "socket closed by server." << endl;
				close(server);
				continue;
			}
			string received = decr_msg((unsigned char*) buf, priv_key);
			if(received == "NULL") {
				cerr << "could not decrypt received msg (reveived)" << endl;
				continue;
			}
			// char * command_enc;
			if(received != "FAILURE."){
				string command_act = "";
				bool force_closed = false;
				do {
					// command_enc = new char[ENC_LEN];
					cout << "action> ";
					getline(cin, command_act);
					command_act = to_lower(command_act);
					smatch withdraw_match;
					smatch deposit_match;
					smatch transfer_match;
					smatch balance_match;
					smatch exit_match;
					regex withdraw_regex(WITHDRAW_REGEX_STR);
					regex deposit_regex(DEPOSIT_REGEX_STR);
					regex transfer_regex(TRANSFER_REGEX_STR);
					regex balance_regex(BALANCE_REGEX_STR);
					regex exit_regex(EXIT_REGEX_STR_CMD);
					regex_search(command_act, withdraw_match, withdraw_regex);
					regex_search(command_act, deposit_match, deposit_regex);
					regex_search(command_act, transfer_match, transfer_regex);
					regex_search(command_act, balance_match, balance_regex);
					regex_search(command_act, exit_match, exit_regex);
					int action = 0;
					string amount_str, t_account;
					if(withdraw_match.size() == 2) {
						action = 1;
						amount_str = withdraw_match[1];
					}
					else if(deposit_match.size() == 2) {
						action = 2;
						amount_str = deposit_match[1];
					}
					else if(transfer_match.size() == 3) {
						action = 3;
						amount_str = transfer_match[1];
						t_account = transfer_match[2];
					}
					else if(balance_match.size() == 1) {
						action = 4;
					}
					else if(exit_match.size() == 1) {
						action = 5;
					}
					else {
						action = -1;
					}
					if(action == 1) {
						char * server_resp = new char[ENC_LEN];
						string payload = "W:" + amount_str;
						if(amount_str.size() > (unsigned int)MAX_AMT_LEN) {
							continue;
						}
						char * command_enc = new char[ENC_LEN];
						command_enc = encr_msg((unsigned char *)payload.c_str(), payload.size(), server_pub_key);
						if(command_enc == NULL) {
							cerr << "could not encrypt command. disconnecting." << endl;
							break;
						}
						send(server, command_enc, ENC_LEN, 0);
						nread = recv(server, server_resp, ENC_LEN, 0);
						if(nread <= 0) {
							cerr << "socket closed by server." << endl;
							force_closed = true;
							close(server);
							break;
						}
						string response = decr_msg((unsigned char*)server_resp, priv_key);
						if(response == "NULL") {
							cerr << "could not decrypt received msg (response)" << endl;
							continue;
						}
						if(response != "FAILURE.") {
							cout << "Withdrawl of " << amount_str << " successful." << endl;	
							cout << "New Balance of " << response << "." << endl;
						}
						else {
							cout << "Withdrawl of " << amount_str << " unsuccessful, insufficient funds." << endl;
						}
					}
					else if(action == 2) {
						char * server_resp = new char[ENC_LEN];
						string payload = "D:" + amount_str;
						if(amount_str.size() > (unsigned int)MAX_AMT_LEN) {
							continue;
						}
						char * command_enc = new char[ENC_LEN];					
						command_enc = encr_msg((unsigned char *)payload.c_str(), payload.size(), server_pub_key);
						if(command_enc == NULL) {
							cerr << "could not encrypt command. disconnecting." << endl;
							break;
						}
						send(server, command_enc, ENC_LEN, 0);
						int nread = recv(server, server_resp, ENC_LEN, 0);
						if(nread <= 0) {
							cerr << "socket closed by server." << endl;
							force_closed = true;
							close(server);
							break;
						}
						string response = decr_msg((unsigned char*)server_resp, priv_key);
						if(response == "NULL") {
							cerr << "could not decrypt received msg (response)" << endl;
							continue;
						}
						if(response != "FAILURE.") {
							cout << "Deposit of " << amount_str << " successful." << endl;	
							cout << "New Balance of " << response << "." << endl;
						}
						else {
							cout << "Deposit of " << amount_str << " unsuccessful." << endl;
						}
					}
					else if(action == 3) {
						char * server_resp = new char[ENC_LEN];
						string payload = "T:" + amount_str + ":" + t_account;
						if(amount_str.size() > (unsigned int)MAX_AMT_LEN || t_account.size() > (unsigned int)MAX_ACT_SIZE) {
							continue;
						}
						char * command_enc = new char[ENC_LEN];					
						command_enc = encr_msg((unsigned char *)payload.c_str(), payload.size(), server_pub_key);
						if(command_enc == NULL) {
							cerr << "could not encrypt command. disconnecting." << endl;
							break;
						}
						send(server, command_enc, ENC_LEN, 0);
						int nread = recv(server, server_resp, ENC_LEN, 0);
						if(nread <= 0) {
							cerr << "socket closed by server." << endl;
							force_closed = true;
							close(server);
							break;
						}
						string response = decr_msg((unsigned char*)server_resp, priv_key);
						if(response == "NULL") {
							cerr << "could not decrypt received msg (response)" << endl;
							continue;
						}
						if(response != "FAILURE.") {
							cout << "Transfer of " << amount_str << " successful." << endl;	
							cout << "New Balance of " << response << "." << endl;
						}
						else {
							cout << "Transfer of " << amount_str << " unsuccessful." << endl;
						}
					}
					else if(action == 4) {
						char * server_resp = new char[ENC_LEN];
						string payload = "B:";
						char * command_enc = new char[ENC_LEN];
						if(command_enc == NULL) {
							cerr << "could not encrypt command. disconnecting." << endl;
							break;
						}
						command_enc = encr_msg((unsigned char *)payload.c_str(), payload.size(), server_pub_key);
						send(server, command_enc, ENC_LEN, 0);
						int nread = recv(server, server_resp, ENC_LEN, 0);
						if(nread <= 0) {
							cerr << "socket closed by server." << endl;
							force_closed = true;
							close(server);
							break;
						}
						string response = decr_msg((unsigned char*)server_resp, priv_key);
						if(response == "NULL") {
							cerr << "could not decrypt received msg (response)" << endl;
							continue;
						}
						if(response != "FAILURE.") {	
							cout << "Current Balance is " << response << "." << endl;
						}
						else {
							cout << "Could not retrieve current balance" << endl;
						}
					}
					else if(action == 5) {
						break;
					}
					else {
						cerr << "Invalid choice." << endl;	
					}					
				}
				while(command_act != "exit");
				if(!force_closed) {
					cout << "logging you out." << endl;
					char * exit_command_enc = new char[ENC_LEN];
					exit_command_enc = encr_msg_str("EXIT.", ENC_LEN, server_pub_key);
					if(exit_command_enc == NULL) {
						cerr << "could not encrypt exit command. disconnecting." << endl;
						break;
					}
					else {
						send(server, exit_command_enc, ENC_LEN, 0);	
					}
					close(server);
				}
			}
			else {
				cerr << "Login Failed." << endl;
			}
		}
	} while(command != "exit");
}
void CryptoClient::close_connection() {
	close(server);
}