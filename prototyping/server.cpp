#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <regex>
#include "server.h"
#include "gen_functions.h"
#include "crypto.h"
#include "bank.h"
#include "constants.h"
using namespace std;
CryptoServer::CryptoServer(int p1, string h1, bool d1, string filename_pub, string filename_priv, string accounts_dir) {
	port = p1;
	host = h1;
	debug = d1;
	reuse = 1;
  is_approved = false;
	clientlen = sizeof(client_addr);
  pub_key = read_keyfile(filename_pub);
  priv_key = read_keyfile(filename_priv);
  bank = Bank(accounts_dir);
}
bool CryptoServer::setup_connection() {
	if(debug) {
    	cout << "DEBUG: listning from " << host << " on port " << port << endl;
	}
	memset(&server_addr, 0, sizeof(server_addr));
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_port = htons(port);
  	server_addr.sin_addr.s_addr = INADDR_ANY;
  	server = socket(PF_INET,SOCK_STREAM,0);
  	if (!server) {
    	perror("socket");
    	return false;
  	}
    struct timeval tv; /* timeval and timeout stuff added by davekw7x */ 
    tv.tv_sec = SOCKET_TIMEOUT; 
    tv.tv_usec = 0; 
    if (setsockopt(server, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof tv)) { 
      perror("setsockopt"); 
      return false; 
    }
  	int setsockopt_rc = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  	if(setsockopt_rc < 0) {
    	perror("setsockopt");
    	return false;
  	}
  	// call bind to associate the socket with our local address and
  	// port
  	int bind_rc = bind(server,(const struct sockaddr *)&server_addr,sizeof(server_addr));
  	if(bind_rc < 0) {
    	perror("bind");
    	return false;
  	}
  	// convert the socket to listen for incoming connections
  	int listen_rc = listen(server, MAXCONNECTIONS);
  	if(listen_rc < 0) {
    	perror("listen");
    	return false;
  	}
  	return true;
}
bool CryptoServer::start_server() {
	bool setup_flag = setup_connection();
	if(!setup_flag) {
		return false;
	}
	while (1) {
		client = accept(server,(struct sockaddr *)&client_addr, &clientlen);
  	if(client < 0) {
    		perror("accept");
    		cerr << "server: dropping client, connected for too long." << endl;
        continue;
  	}
    // process_connection(client);
  	int pid = fork();
  	if(pid < 0) {
    		perror("fork");
    		exit(EXIT_FAILURE);
  	}
  	if(pid == 0) {
        if(debug) {
          char str[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &(client_addr.sin_addr), str, INET_ADDRSTRLEN);
          cout << "server: a connection from " << str << endl;
        }
        close(server);
    		process_connection(client);
    		exit(EXIT_SUCCESS);
  	}
  	else {
  		close(client);
  	}
	}
}
void CryptoServer::process_connection(int sock) {
	char * buf = new char[BUF_LEN + 1];
	while(1) {
  	memset(buf, 0, BUF_LEN);
  	int nread = recv(sock, buf, BUF_LEN, 0);
  	if(nread == 0) {
    		cout << "server: got a read length of 0, closing socket" << endl;
    		close(sock);
    		return;
  	}
    else if(nread < 0) {
      cerr << "server: idle too long, client timed out" << endl;
      close(sock);
      return;
    }
    if(strcmp(buf, "PUBKEY") == 0 && !is_approved) {
      // assume its a good key for now,
      //  MAC stuff will go here, we will have a list
      //  of public keys that are associated with good
      //  private keys. Will need Mersenne twister.
      char * buf_key = new char[BUF_LEN];
      send(sock, pub_key.c_str(), pub_key.size(), 0);
      nread = recv(sock, buf_key, BUF_LEN, 0);
      if(nread == 0) {
        cout << "server: got a read length of 0, closing socket" << endl;
        close(sock);
        return;
      }
      else if(nread < 0) {
        cerr << "server: idle too long, client timed out" << endl;
        close(sock);
        return;
      }
      clinet_pub_key = (string)buf_key;
      cout << "server: secure connection established." << endl;
      char * ok_enc = new char[ENC_LEN];
      char * fail_enc = new char[ENC_LEN];
      char * success_enc = new char[ENC_LEN];
      ok_enc = encr_msg_str("OK.", 10, clinet_pub_key);
      if(ok_enc == NULL) {
        cerr << "server: could not encrypt ok_enc, closing socket." << endl;
        close(sock);
        exit(EXIT_FAILURE);
      }
      fail_enc = encr_msg_str("FAILURE.", 10, clinet_pub_key);
      if(ok_enc == NULL) {
        cerr << "server: could not encrypt fail_enc, closing socket." << endl;
        close(sock);
        exit(EXIT_FAILURE);
      }
      success_enc = encr_msg_str("SUCCESS.", 10, clinet_pub_key);
      if(ok_enc == NULL) {
        cerr << "server: could not encrypt success_enc, closing socket." << endl;
        close(sock);
        exit(EXIT_FAILURE);
      }
      send(sock, (unsigned char*)ok_enc, ENC_LEN, 0);
      char * login_info_enc = new char[ENC_LEN];
      regex act_regex(ACT_REGEX_STR);
      nread = recv(sock, login_info_enc, ENC_LEN, 0);
      if(nread == 0) {
        cout << "server: got a read length of 0, closing socket." << endl;
        close(sock);
        return;
      }
      else if(nread < 0) {
        cerr << "server: idle too long, client timed out." << endl;
        close(sock);
        return;
      }
      string login_info_dec = decr_msg((unsigned char*)login_info_enc, priv_key);
      if(login_info_dec == "NULL") {
        cerr << "server: could not decrypt received msg (login_info_dec)." << endl;
        break;
      }
      smatch login_regex_result;
      string login_info_str(login_info_dec);
      regex_search(login_info_str, login_regex_result, act_regex);
      string act = login_regex_result[1];
      int pin = atoi(str_to_char_ptr_safe(login_regex_result[2], MAX_PIN_SIZE));
      cout << "server: Attempting login...." << endl;
      bool login_success = bank.lock_act(act, pin);
      bool do_exit = false;
      if(!login_success) {
        cout << "server: login failed." << endl;
        send(sock, fail_enc, ENC_LEN, 0);
        break;
      }
      else {
        cout << "server: user " << act << " logged in successfully." << endl;
        send(sock, success_enc, ENC_LEN, 0);
        while(!do_exit) {
          char * transaction_enc = new char[ENC_LEN];
          memset( transaction_enc, '0', sizeof(char)*ENC_LEN );
          nread = recv(sock, transaction_enc, ENC_LEN, 0);
          if(nread == 0) {
            cout << "server: got a read length of 0, closing socket." << endl;
            break;
          }
          else if(nread < 0) {
            cerr << "server: idle too long, client timed out." << endl;
            close(sock);
            exit(EXIT_FAILURE);
          }
          // TODO: Fix sometimes getting garbage values on decrypt
          string tmp = decr_msg((unsigned char*)transaction_enc, priv_key);
          if(tmp == "NULL") {
            cerr << "server: could not decrypt received msg (tmp)." << endl;
            continue;
          }
          regex trans_regex_non_balance_transfer(TRANS_REGEX_STR_NON_BALANCE_TRANSFER);
          regex trans_regex_balance(TRANS_REGEX_STR_BALANCE);
          regex trans_regex_transfer(TRANS_REGEX_STR_TRANSFER);
          smatch trans_regex_non_balance_transfer_result;
          smatch trans_regex_balance_result;
          smatch trans_regex_transfer_result;
          regex_search(tmp, trans_regex_non_balance_transfer_result, trans_regex_non_balance_transfer);
          regex_search(tmp, trans_regex_balance_result, trans_regex_balance);
          regex_search(tmp, trans_regex_transfer_result, trans_regex_transfer);
          if(tmp == "EXIT.") {
            cout << "server: user " << act << " logged out." << endl;
            break;
          }
          else if(trans_regex_non_balance_transfer_result.size() != 3 && trans_regex_balance_result.size() != 1 && trans_regex_transfer_result.size() != 3) {
            cout << "merp!" << endl;
            send(sock, fail_enc, ENC_LEN, 0);
          }
          if(trans_regex_non_balance_transfer_result.size() == 3) {
            if(trans_regex_non_balance_transfer_result[1] == "W") {
              int amount = atoi(str_to_char_ptr_safe(trans_regex_non_balance_transfer_result[2], MAX_AMT_LEN));
              bool withdraw_success = bank.withdraw(act, pin, amount);
              if(withdraw_success) {
                cerr << "server " << act << ": withdrawl of " << amount << " from " << act << " successful." << endl;
                char * new_balance_enc = new char[ENC_LEN];
                new_balance_enc = encr_msg_int(bank.balance_inq(act, pin), MAX_AMT_LEN, clinet_pub_key);
                if(new_balance_enc == NULL) {
                  cerr << "could not enc new_balance_enc. closing socket." << endl;
                  close(sock);
                  exit(EXIT_FAILURE);
                }
                send(sock, new_balance_enc, ENC_LEN, 0);
              }
              else {
                cerr << "server " << act << ": withdrawl of " << amount << " from " << act << " unsuccessful." << endl;
                send(sock, fail_enc, ENC_LEN, 0); 
              }
            }
            else if(trans_regex_non_balance_transfer_result[1] == "D") {
              int amount = atoi(str_to_char_ptr_safe(trans_regex_non_balance_transfer_result[2], MAX_AMT_LEN));
              bool withdraw_success = bank.deposit(act, pin, amount);
              if(withdraw_success) {
                cerr << "server " << act << ": deposit of " << amount << " to " << act << " successful." << endl;
                char * new_balance_enc = new char[ENC_LEN];
                new_balance_enc = encr_msg_int(bank.balance_inq(act, pin), MAX_AMT_LEN, clinet_pub_key);
                if(new_balance_enc == NULL) {
                  cerr << "server: could not enc new_balance_enc. closing socket." << endl;
                  close(sock);
                  exit(EXIT_FAILURE);
                }
                send(sock, new_balance_enc, ENC_LEN, 0);
              }
              else {
                cerr << "server " << act << ": deposit of " << amount << " to " << act << " unsuccessful." << endl;
                send(sock, fail_enc, ENC_LEN, 0); 
              }
            }
          }
          else if(trans_regex_balance_result.size() == 1) {
            int amount = bank.balance_inq(act, pin);
            if(amount != -1) {
              cout << "server " << act << ": balance inquery successful." << endl;
              char * new_balance_enc = new char[ENC_LEN];
              new_balance_enc = encr_msg_int(amount, MAX_AMT_LEN, clinet_pub_key);
              if(new_balance_enc == NULL) {
                cerr << "server: could not enc new_balance_enc. closing socket." << endl;
                close(sock);
                exit(EXIT_FAILURE);
              }
              send(sock, new_balance_enc, ENC_LEN, 0);
            }
            else {
              cout << "server " << act << ": balance Inquery unsuccessful.";
              send(sock, fail_enc, ENC_LEN, 0); 
            }
          }
          else if(trans_regex_transfer_result.size() == 3) {
            int amount = atoi(str_to_char_ptr_safe(trans_regex_transfer_result[1], MAX_AMT_LEN));
            string t_account = trans_regex_transfer_result[2];
            bool transfer_success = bank.transfer(act, pin, t_account, amount);
            if(transfer_success) {
              cout << "server " << act << ": transfer of " << amount << " to act " << t_account << " successful." << endl;
              char * new_balance_enc = new char[ENC_LEN];
              new_balance_enc = encr_msg_int(bank.balance_inq(act, pin), MAX_AMT_LEN, clinet_pub_key);
              if(new_balance_enc == NULL) {
                cerr << "server: could not enc new_balance_enc. closing socket." << endl;
                close(sock);
                exit(EXIT_FAILURE);
              }
              send(sock, new_balance_enc, ENC_LEN, 0);
            }
            else {
              cerr << "server " << act << ": transfer of " << amount << " to act " << t_account << " unsuccessful." << endl;
              send(sock, fail_enc, ENC_LEN, 0); 
            }
          }
        }
        bank.unlock_act(act, pin);
        close(sock);
        exit(EXIT_SUCCESS);
      }
    }
    else {
      close(sock);
    }
  }
}
void CryptoServer::send_public_key(int sock) {
  send(sock, pub_key.c_str(), pub_key.size(), 0);
}