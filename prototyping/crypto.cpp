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
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>
#include <openssl/evp.h> 
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "gen_functions.h"
using namespace std;
RSA * create_rsa_pub(char * key) {
  RSA * rsa = NULL;
  BIO * keybio;
  keybio = BIO_new_mem_buf(key, -1);
  if (keybio == NULL) {
    cerr << "crypto: failed to create key BIO" << endl;
    // error check
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
    cerr << "crypto: failed to create key BIO" << endl;
    // error check
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
  memset(key, '\0', BUF_LEN * sizeof(char));
  strncpy(key, priv_key.c_str(), priv_key.size());
  RSA * rsa = create_rsa_priv(key);
  char * decr = new char[BUF_LEN];
  memset(decr, '\0', BUF_LEN * sizeof(char));
  int result = private_decrypt(rsa, msg, ENC_LEN, (unsigned char*)key, (unsigned char*)decr);
  if(result == -1) {
    cerr << "crypto: private Decrypt failed " << endl;
    // error check
    return "NULL";
  }
  string decr_str = string(decr);
  return decr_str;
}
char * encr_msg(unsigned char * msg, int msg_len, string pub_key) {
	char * key = new char[BUF_LEN];
  memset(key, '\0', BUF_LEN * sizeof(char));
	strncpy(key, pub_key.c_str(), pub_key.size());
	RSA * rsa = create_rsa_pub(key);
	char * enc = new char[ENC_LEN];
  memset(enc, '\0', ENC_LEN * sizeof(char));
	int result = public_encrypt(rsa, msg, msg_len, (unsigned char*)key, (unsigned char *)enc);
	if(result == -1) {
		cerr << "crypto: public Encrypt failed " << endl;
    // error check
		return NULL;
	}
	return enc;
}
char * encr_msg_str(string msg_str, int max_size, string pub_key) {
  int msg_len;
  if(msg_str.size() > (unsigned int)max_size) {
    // error check
    return NULL;
  }
  else {
    msg_len = msg_str.size();
  }
  char * msg = new char[ENC_LEN];
  memset(msg, '\0', ENC_LEN * sizeof(char));
  strncpy(msg, msg_str.c_str(), msg_str.size());
  return encr_msg((unsigned char*)msg, msg_len, pub_key);
}
char * encr_msg_int(int msg_int, int max_size, string pub_key) {
  int msg_len;
  string msg_str = to_string(msg_int);
  if(msg_str.size() > (unsigned int)max_size) {
    // error check
    return NULL;
  }
  else {
    msg_len = msg_str.size();
  }
  char * msg = new char[ENC_LEN];
  memset(msg, '\0', ENC_LEN * sizeof(char));
  strncpy(msg, msg_str.c_str(), msg_str.size());
  return encr_msg((unsigned char*)msg, msg_len, pub_key);
}

string get_mac(string msg, string key) {
  unsigned int md_len = 0; // length of MAC/digest
  char * mdS = new char[EVP_MAX_MD_SIZE];
  unsigned char * mac_ptr = new unsigned char[EVP_MAX_MD_SIZE];
  memset(mdS, '\0', EVP_MAX_MD_SIZE * sizeof(char));
  memset(mac_ptr, '\0', EVP_MAX_MD_SIZE * sizeof(char));
  mac_ptr = HMAC(EVP_sha512(), (unsigned char*)key.c_str(), key.size(), (unsigned char*)msg.c_str(), msg.size(), (unsigned char*)mdS, &md_len);
  string mac_str = raw_to_hex((unsigned char*)mdS, md_len);
  return mac_str;
}
string make_random_msg() {
  char * rand_msg = new char[MAC_MSG_LEN];
  memset(rand_msg, '\0', MAC_MSG_LEN * sizeof(char));
  int rc = RAND_bytes((unsigned char *)rand_msg, MAC_MSG_LEN);
  if(rc == 0) {
    cerr << "crypto: failed to make random msg." << endl;
    exit(EXIT_FAILURE);
  }
  string rnd_msg_str = raw_to_hex((unsigned char*)rand_msg, MAC_MSG_LEN);
  return rnd_msg_str;
}

