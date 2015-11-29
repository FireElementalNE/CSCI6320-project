#include <iostream>
#include <openssl/rsa.h>

extern RSA * create_rsa_pub(char * key); 
extern RSA * create_rsa_priv(char * key);
extern int private_decrypt(RSA * rsa, unsigned char * enc_data, int data_len,unsigned char * key, unsigned char *decrypted);
extern int public_encrypt(RSA * rsa, unsigned char * data, int data_len, unsigned char * key, unsigned char * encrypted);
extern string decr_msg(unsigned char * msg, string priv_key);
extern char * encr_msg(unsigned char * msg, int msg_len, string pub_key);