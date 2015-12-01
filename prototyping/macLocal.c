//Jassiem Ifill
//csci6320 proj
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/hmac.h>
#include <openssl/evp.h> //high level interaces for message digests
#include <openssl/engine.h>

//NOTE: compile with: 'gcc macLocal.c -lcrypto' (Need to link shared library)

//HMAC - computes message authentiication code of length n bytes starting at location d(begining of message)
/*
- evp_md can be EVP_sha1(), EVP_ripemd160(), etc (list of avialable hash functions? likely)
 */
//HMAC_CTX_init(): initializes 'a HMAC_CTX' (must be called before use)

void printMac(const unsigned char * msg,const char * key,int mode);

unsigned char *HMACRSA(const EVP_MD *evp_md, const void *key, int key_len,const unsigned char *d, size_t n, unsigned char *md,unsigned int *md_len);

int main(int argc, char *argv[])
{
  const char keyString[]  = "455554";
  const unsigned char testMessage[] = "Hello world!";
 //Note! Digest is the mac, actual length of the MAC is placed in md_len (named rc in this code), do not use strlen
  //  printMac(testMessage,keyString,0);
  // printMac((unsigned char *)"This is a message...",keyString,0);
  // printMac((unsigned char *)"Hello world!",keyString,0);
  if(argc>1) //passing public key as argument to use as key to hmac,tested with public key of server
    {
      struct stat sb;
      int rc = stat(argv[1],&sb);
      if(rc == -1)
	return EXIT_FAILURE;
      fprintf(stderr,"size of %s is: %li bytes\n",argv[1],sb.st_size);

      //      return EXIT_SUCCESS;
      FILE *fp = fopen(argv[1],"r");
      if(!fp)
	return EXIT_FAILURE;
      //can also be used to get size//
      /*
      long lSize;
      fseek (fp , 0 , SEEK_END);
      lSize = ftell (fp);
      rewind (fp);
//      fprintf(stderr,"size of %s is: %li bytes\n",argv[1],lSize);
      */
      char * keyBuff = calloc(sb.st_size,sizeof(char));
      rc = fread(keyBuff,sizeof(char),sb.st_size,fp);
      fclose(fp);
      if(rc!= sb.st_size)
	fprintf(stderr,"Incorrect number of bytes read\n");
      else
	fprintf(stdout,"Contents of %s:\n%s\n",argv[1],keyBuff);

      printMac((unsigned char *)"Hello world!",keyBuff,0);
      printMac((unsigned char *)"Hello world!",keyBuff,0);
      free(keyBuff);
    }

  return EXIT_SUCCESS;
}

void printMac(const unsigned char * msg,const char * key,int mode)
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
  sprintf(buff,"MAC of '%s', with key =\n%s\n is: \n\t",msg,key);
  //EVP_MD alg = 
  //  fprintf(stderr,"Before hmac attempt---\n");
  if(mode==0)
    macPtr =  HMAC(EVP_sha1(),key,strlen(key),msg,strlen((char *)msg),mdS,&md_len);
  else//ignore
    macPtr =  HMACRSA(EVP_sha1(),key,strlen(key),msg,strlen((char *)msg),mdS,&md_len);
  temp = macPtr;
 // temp = mdS;
  fprintf(stdout,"MAC length = %i\n",md_len);
  for(temp = mdS;i<md_len;temp++,i++)
    fprintf(stdout,"%s%u %s",(i==0)?buff:"",(unsigned int)*temp,(i==(md_len-1))?"\n":"");

  fprintf(stdout,"MAC length = %i\n",md_len);
  for(i=0,temp = mdS;i<md_len;temp++,i++)
    fprintf(stdout,"%s%x%s",(i==0)?buff:"",(unsigned int)*temp,(i==(md_len-1))?"\n":"");

// fprintf(stderr,"After hmac attempt --\n");

}


//ignore
unsigned char *HMACRSA(const EVP_MD *evp_md, const void *key, int key_len,const unsigned char *d, size_t n, unsigned char *md,unsigned int *md_len)
{
    HMAC_CTX c;
    static unsigned char m[EVP_MAX_MD_SIZE];
    //TODO: get/set rsa engine struct
    const char *engine_id = "rsa";
    ENGINE_load_builtin_engines();
    ENGINE *e = ENGINE_by_id(engine_id);//ENGINE_;
    if(!e)
      fprintf(stderr,"Engine not available\n");

    ENGINE_init(e);
    if (md == NULL)
        md = m;
    HMAC_CTX_init(&c);

    if (!HMAC_Init_ex(&c, key, key_len, evp_md, NULL))
        goto err;
    if (!HMAC_Update(&c, d, n))
        goto err;
    if (!HMAC_Final(&c, md, md_len))
        goto err;
    HMAC_CTX_cleanup(&c);
    ENGINE_free(e);
    return md;
 err:
    HMAC_CTX_cleanup(&c);
    ENGINE_free(e);
    return NULL;
}
