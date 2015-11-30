#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <regex>
#include "server.h"
#include "gen_functions.h"
#include "crypto.h"
#include "bank.h"
#include "constants.h"
using namespace std;
CryptoServer::CryptoServer(int p1, string h1, bool d1, string filename_pub, string filename_priv, string accounts_filename) {
	port = p1;
	host = h1;
	debug = d1;
	reuse = 1;
  is_approved = false;
	clientlen = sizeof(client_addr);
  pub_key = read_keyfile(filename_pub);
  priv_key = read_keyfile(filename_priv);
  bank = Bank(accounts_filename);
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
    		exit(EXIT_FAILURE);
  	}
  	if(debug) {
    		char str[INET_ADDRSTRLEN];
    		inet_ntop(AF_INET, &(client_addr.sin_addr), str, INET_ADDRSTRLEN);
    		cout << "Got a connection from " << str << endl;
  	}
  	int pid = fork();
  	if(pid < 0) {
    		perror("fork");
    		exit(EXIT_FAILURE);
  	}
  	if(pid == 0) {
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
    		cout << "GOT a read length of 0, closing socket" << endl;
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
      recv(sock, buf_key, BUF_LEN, 0);
      clinet_pub_key = (string)buf_key;
      // TODO: I am going to get rid of this boolean
      //   causing some werid behavior
      is_approved = false;
      cout << "Secure Connection Established." << endl;
      char * ok_enc = new char[ENC_LEN];
      char * fail_enc = new char[ENC_LEN];
      char * success_enc = new char[ENC_LEN];
      ok_enc = encr_msg_str("OK.", clinet_pub_key);
      fail_enc = encr_msg_str("FAILURE.", clinet_pub_key);
      success_enc = encr_msg_str("SUCCESS.", clinet_pub_key);
      send(sock, (unsigned char*)ok_enc, ENC_LEN, 0);
      // char * act_enc = new char[ENC_LEN];
      // char * pin_enc = new char[ENC_LEN];
      char * login_info_enc = new char[ENC_LEN];
      regex rgx1("^(\\d+):(\\d+)$");
      recv(sock, login_info_enc, ENC_LEN, 0);
      string login_info_dec = decr_msg((unsigned char*)login_info_enc, priv_key);
      smatch result1;
      string login_info_str(login_info_dec);
      cout << login_info_str << endl;
      regex_search(login_info_str, result1, rgx1);
      for(unsigned int i = 0; i < result1.size(); i++) {
        cout << result1[i] << endl;
      }
      int act = atoi(str_to_char_ptr_safe(result1[1], MAX_ACT_SIZE));
      int pin = atoi(str_to_char_ptr_safe(result1[2], MAX_PIN_SIZE));
      cout << "sizse: " << result1.size() << endl;
      cout << "Account #: " << act << endl;
      cout << "Pin:  " << pin << endl;;
      cout << "Attempting login....";
      bool login_success = bank.lookup_account(act, pin);
      cout << login_success << endl;
      if(!login_success) {
        send(sock, fail_enc, ENC_LEN, 0);
        close(sock);
      }
      else {
        cout << "SUCCESS." << endl;
        send(sock, success_enc, ENC_LEN, 0);
        char * transaction_enc;
        char * transaction_dec;
        do {
          transaction_dec = new char[ENC_LEN];
          transaction_enc = new char[ENC_LEN];
          recv(sock, transaction_enc, ENC_LEN, 0);
          string tmp = decr_msg((unsigned char*)transaction_enc, priv_key);
          regex rgx("^([A-Z]):(\\d+)$");
          smatch result;
          regex_search(tmp, result, rgx);
          cout << result.size() << endl;
          cout << result[1] << endl;
          if(result.size() != 3) {
            send(sock, fail_enc, ENC_LEN, 0);
          }
          if(result[1] == "W") {
            int amount = atoi(str_to_char_ptr_safe(result[2], MAX_AMT_LEN));
            cout << "account before: " << bank.balance_inq(act, pin) << endl;
            bool withdraw_success = bank.withdraw(act, pin, amount);
            cout << "account after: " << bank.balance_inq(act, pin) << endl;
            if(withdraw_success) {
              send(sock, success_enc, ENC_LEN, 0);
            }
            else {
             send(sock, fail_enc, ENC_LEN, 0); 
            }
          }
          
        } while(strcmp(transaction_dec, "EXIT.") != 0);
        close(sock);
      }
    }
    else if(is_approved){
      string msg = decr_msg((unsigned char *)buf, priv_key);
      if(msg == "") {
        cerr << "ERROR: decryption failed. Sorry." << endl;
        close(sock);
        exit(EXIT_FAILURE);
      }
      if(debug) {
        string hex_tmp = raw_to_hex((unsigned char *)buf, ENC_LEN);
        cout << "DEBUG: got '" << hex_tmp << "' from client." << endl;
      }
      char * tmp = new char[msg.size()];
      char * enc_msg = new char[ENC_LEN];
      strncpy(tmp, msg.c_str(), msg.size());
      enc_msg = encr_msg((unsigned char*)tmp, msg.size(), clinet_pub_key);
      send(sock, enc_msg, ENC_LEN, 0);
      if(debug) {
        string hex_tmp = raw_to_hex((unsigned char *)enc_msg, ENC_LEN);
        cout << "DEBUG: sent '" << hex_tmp << "' to client." << endl;
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