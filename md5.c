
	#include <stdio.h>

	#include <stdlib.h>

	#include <unistd.h>

	#include <sys/types.h>

	#include <sys/wait.h>

	#include "header.h"

	#include <sys/ipc.h>

	#include <sys/sem.h>

	#include <sys/shm.h>

	#include <sys/signal.h>

	#include <sys/times.h>

	#include <errno.h>

	#include <time.h>

	#include <string.h>

	#include <openssl/md5.h>

	#include <stdint.h>
	
	#include <openssl/hmac.h>

	#include <semaphore.h>

	//#include <CommonCrypto/CommonDigest.h>
	typedef unsigned long int uint32;
	struct MD5Context {
    uint32 buf[4];
    uint32 bits[2];
    unsigned char in[64];
	};

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void MD5Init(struct MD5Context *context);

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
void MD5Update(struct MD5Context *context, unsigned char const *buf, unsigned len);

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern 
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
void MD5Final(unsigned char digest[16], struct MD5Context *context);

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
void MD5Transform(uint32 buf[4], uint32 const in[16]);



int main(){
unsigned char digest[16];
const char* string = "Hello World";
struct MD5Context context;
MD5Init(&context);
MD5Update(&context, string, strlen(string));
MD5Final(digest, &context);


char md5string[33];
for(int i = 0; i < 16; ++i)
sprintf(&md5string[i*2], "%02x", (unsigned int)digest[i]);

}