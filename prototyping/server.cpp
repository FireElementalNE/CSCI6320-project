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
#include "server.h"
#include "gen_functions.h"
#include "crypto.h"
#include "bank.h"
#define BUF_LEN 4096
#define MAXCONNECTIONS 5
#define ENC_LEN 256 // length of encr msg
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
      send(sock, pub_key.c_str(), BUF_LEN, 0);
      recv(sock, buf_key, BUF_LEN, 0);
      clinet_pub_key = (string)buf_key;
      is_approved = true;
      cout << "Secure Connection Established." << endl;
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
      send(sock, enc_msg, BUF_LEN, 0);
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
  send(sock, pub_key.c_str(), BUF_LEN, 0);
}