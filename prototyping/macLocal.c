//Jassiem Ifill
//csci6320 proj
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/evp.h> //high level interaces for message digests

//NOTE: compile with: 'gcc macLocal.c -lcrypto' (Need to link shared library)

//prototypes
//HMAC - computes message authentiication code of length n bytes starting at location d(begining of message)
/*

- key is key_len bytes long
- md is the string that recieves the output of the hash (must have space allocated for output of hash, no longer than EVP_MAX_MD_SIZE bytes)
- if md is NULL, digest is placed in a static array
- size of hash output is placed in md_len, unless the the size is NULL (0?)
- evp_md can be EVP_sha1(), EVP_ripemd160(), etc (list of avialable hash functions? likely)
 unsigned char *HMAC(
const EVP_MD *evp_md, //
const void *key,
 int key_len, 
const unsigned char *d, 
int n,
 unsigned char *md, 
unsigned int *md_len);

 */
//HMAC_CTX_init(): initializes 'a HMAC_CTX' (must be called before use)

void printMac(const unsigned char * msg,const char * key);

int main(int argc, char *argv[])
{

  // HMAC_CTX *ctx; //hmac context?
  const char keyString[]  = "455554";
  //  int dummyKey =  0x455554;
  const unsigned char testMessage[] = "Hello world!";
  //  unsigned char * md; //to hold message digest  
  //  void HMAC_CTX_init(ctx);
  //  unsigned char mdS[EVP_MAX_MD_SIZE];
  //  unsigned int rc = 0;
  //  unsigned char * macPtr;
  //EVP_MD alg = 
  //  fprintf(stderr,"Before hmac attempt---\n");
  //macPtr =  HMAC(EVP_sha1(),keyString,strlen(keyString),testMessage,strlen((char *)testMessage),mdS,&rc);

  // fprintf(stderr,"After hmac attempt --\n");
//  fprintf(stdout,"Mac of %s (size %i) is:%s (len = %i)\n",testMessage,(int)strlen((char *)testMessage),mdS,(int)rc);

// fprintf(stdout,"%s\n",(char *)macPtr);
//print MAC
  /*
 int i = 0;
 unsigned char* temp = macPtr;
 int len = strlen((char *)macPtr);
 fprintf(stdout,"len = %i, rc(md_len) = %u\n",len,rc);
 for(temp = macPtr;i<len;temp++,i++)
   fprintf(stdout,"%s%u %s",(i==0)?"MAC: \n\t":"",(unsigned int)*temp,(i==(len-1))?"\n":"");

 for(i=0,temp = mdS;i<rc;temp++,i++)
   fprintf(stdout,"%s%u %s",(i==0)?"Digest: \n\t":"",(unsigned int)*temp,(i==(rc-1))?"\n":"");
  */
 //Note! Digest is the mac, actual length of the MAC is placed in md_len (named rc in this code), do not use strlen
 printMac(testMessage,keyString);
 printMac((unsigned char *)"This is a message...",keyString);
 printMac((unsigned char *)"Hello world!",keyString);

  return EXIT_SUCCESS;
}

void printMac(const unsigned char * msg,const char * key)
{
 const char keyString[]  = "455554";
  int dummyKey =  0x455554;
  unsigned int md_len = 0; //length of MAC/digest
  unsigned char mdS[EVP_MAX_MD_SIZE];
  unsigned int rc = 0;
  unsigned char * macPtr;
  unsigned char* temp;
  int i = 0;
  char buff[BUFSIZ] = {0};
  sprintf(buff,"MAC of '%s': \n\t",msg);
  //EVP_MD alg = 
  //  fprintf(stderr,"Before hmac attempt---\n");
  macPtr =  HMAC(EVP_sha1(),key,strlen(key),msg,strlen((char *)msg),mdS,&md_len);
  temp = macPtr;
 // temp = mdS;
  fprintf(stdout,"MAC length = %i\n",md_len);
  for(temp = mdS;i<md_len;temp++,i++)
    fprintf(stdout,"%s%u %s",(i==0)?buff:"",(unsigned int)*temp,(i==(md_len-1))?"\n":"");

// fprintf(stderr,"After hmac attempt --\n");

}

