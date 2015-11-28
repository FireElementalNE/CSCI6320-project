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
#define BUF_LEN 4096
#define MAXCONNECTIONS 5
CryptoServer::CryptoServer(int p1, string h1, bool d1, string filename_pub, string filename_priv) {
	port = p1;
	host = h1;
	debug = d1;
	reuse = 1;
	clientlen = sizeof(client_addr);
  pub_file = filename_pub;
  priv_file = filename_priv;
  is_enc = false;
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
    /*if(is_enc) {
      buf = decr_msg(buf);
    }*/
  	if(nread == 0) {
    		cout << "GOT a read length of 0, closing socket" << endl;
    		close(sock);
    		return;
  	}
  	else {
    		if(debug) {
      		cout << "DEBUG: got '" << buf << "' from client." << endl;
    		}
  	}
    if(strcmp(buf, "PUBKEY") == 0) {
      is_enc = true;
      send_public_key(sock);
      char * buf_encr = new char[BUF_LEN + 1];
      int nread = recv(sock, buf_encr, BUF_LEN, 0);
      cout << "got encr: " << buf_encr << endl;
      string to_send = decr_msg((unsigned char *)buf_encr);
      send(sock, to_send.c_str(), BUF_LEN, 0);
    }
    else {
      send(sock, buf, BUF_LEN, 0);
      if(debug) {
        cout << "DEBUG: sent '" << buf << "' to client." << endl;
      }  
    }
  }
}
string CryptoServer::read_priv_key() {
  ifstream priv;
  priv.open(priv_file.c_str());
  if(!priv.is_open()) {
    cerr << "could not open priv key " << pub_file << endl;
    return "";
  }
  string str((istreambuf_iterator<char>(priv)), istreambuf_iterator<char>());
  priv.close();
  return str;
}
RSA * CryptoServer::create_rsa_priv(char * key) {
  RSA * rsa = NULL;
  BIO * keybio;
  keybio = BIO_new_mem_buf(key, -1);
  if (keybio == NULL) {
    cerr << "Failed to create key BIO" << endl;
    return NULL;
  }
  rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
  return rsa;
}
int CryptoServer::private_decrypt(RSA * rsa, unsigned char * enc_data, int data_len,unsigned char * key, unsigned char *decrypted) {
  if(debug) {
    cout << "CryptoServer::private_decrypt: Got a msg of len " << "fef" << endl;
  }
  int padding = RSA_PKCS1_PADDING;
  int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
  return result;
}
string CryptoServer::decr_msg(unsigned char * msg) {
  string priv_key = read_priv_key();
  char * key = new char[BUF_LEN];
  key = str_to_unsigned_char_ptr(priv_key);
  RSA * rsa = create_rsa_priv(key);
  char * decr = new char[BUF_LEN];
  int result = private_decrypt(rsa, msg, ENC_LEN, (unsigned char*)key, (unsigned char*)decr);
  if(result == -1) {
    cerr << "Private Decrypt failed " << endl;
  }
  else {
    cout << "DECR:" << endl << decr << endl;
  }
  return (string)decr;
}
void CryptoServer::send_public_key(int sock) {
  ifstream pub;
  pub.open(pub_file.c_str());
  if(!pub.is_open()) {
    cerr << "could not open pub key " << pub_file << endl;
    close(sock);
  }
  string str((istreambuf_iterator<char>(pub)), istreambuf_iterator<char>());
  pub.close();
  send(sock, str.c_str(), BUF_LEN, 0);
}