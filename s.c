#include "md5.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* hashfunc (char* word1, char* word2){
    //concat both words
    strncat(word1,word2,100);
    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5,word1,strlen(word1));
    unsigned char* digest;
    MD5_Final(digest,&md5);

    return digest;
}

int main(){
    char* a = "dick";
    char* b = "duenn";

    char* ret = hashfunc(a,b);
    printf("MD5 hash: %s\n",ret);
}
