#include <stdio.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <time.h>


int main()
{
    
        //use unsigned char
        unsigned char digest[16];
        char *input;
        strcpy(input,buf);
        //char *input = buf;
        printf("input is %s\n",input);
        int length = strlen(input) -1 ;
        printf("length is %d\n",length);
        int ii=0;

        // don't miss the underscore after MD5
        MD5_CTX md5;    
        MD5_Init(&md5);

        while (length > 0) {
            if (length > 512) {
                MD5_Update(&md5, input, 512);
            } else {
                MD5_Update(&md5, input, length);
            }
            length -= 512;
            input += 512;
        }

        MD5_Final(digest, &md5);
        printf("digest is: ");
        for(ii = 0; ii < 16; ii++) {
            printf("%02x", digest[ii]);
        }
        printf("\n");
        
}