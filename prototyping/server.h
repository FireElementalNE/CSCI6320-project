#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <openssl/rsa.h>
using namespace std;
class CryptoServer {
private:
	int port;
	string host;
	struct sockaddr_in server_addr, client_addr;
	socklen_t clientlen;
	int reuse;
  	int server, client;
  	bool debug;
  	string pub_file;
  	string priv_file;
  	bool is_enc;
  	bool setup_connection();
  	void process_connection(int sock);
  	void send_public_key(int sock);
  	string read_priv_key();
  	RSA * create_rsa_priv(char * key);
  	int private_decrypt(RSA * rsa, unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted);
  	string decr_msg(unsigned char * msg);
public:
	CryptoServer(int p1, string h1, bool d1, string filename_pub, string filename_priv);
	bool start_server();
};