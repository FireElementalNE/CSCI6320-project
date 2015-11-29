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
#define BUF_LEN 4096
#define ENC_LEN 256 // length of encr msg
using namespace std;
RSA * create_rsa_pub(char * key) {
  RSA * rsa = NULL;
  BIO * keybio;
  keybio = BIO_new_mem_buf(key, -1);
  if (keybio == NULL) {
    cerr << "Failed to create key BIO" << endl;
    return NULL;
  }
  rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
  return rsa;
}
RSA * create_rsa_priv(char * key) {
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
int private_decrypt(RSA * rsa, unsigned char * enc_data, int data_len,unsigned char * key, unsigned char *decrypted) {
  int padding = RSA_PKCS1_PADDING;
  int  result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, padding);
  return result;
}
int public_encrypt(RSA * rsa, unsigned char * data, int data_len, unsigned char * key, unsigned char * encrypted) {
	int padding = RSA_PKCS1_PADDING;
    int result = RSA_public_encrypt(data_len, data, encrypted, rsa, padding);
    return result;
}
string decr_msg(unsigned char * msg, string priv_key) {
  char * key = new char[BUF_LEN];
  strncpy(key, priv_key.c_str(), priv_key.size());
  RSA * rsa = create_rsa_priv(key);
  char * decr = new char[BUF_LEN];
  int result = private_decrypt(rsa, msg, ENC_LEN, (unsigned char*)key, (unsigned char*)decr);
  if(result == -1) {
    cerr << "Private Decrypt failed " << endl;
    return "";
  }
  return (string)decr;
}
char * encr_msg(unsigned char * msg, int msg_len, string pub_key) {
	char * key = new char[BUF_LEN];
	strncpy(key, pub_key.c_str(), pub_key.size());
	RSA * rsa = create_rsa_pub(key);
	char * enc = new char[BUF_LEN];
	int result = public_encrypt(rsa, msg, msg_len, (unsigned char*)key, (unsigned char *)enc);
	if(result == -1) {
		cerr << "Public Encrypt failed " << endl;
		exit(EXIT_FAILURE);
	}
	return enc;
}