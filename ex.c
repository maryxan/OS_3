#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
FILE   *fp;
char   buf[128];
int    flines;    /* number fo lines in file */
int    rline;     /* the random line to read */
int    i;
 
 
srand(time(NULL));
if ( ! ( fp = fopen("atext.txt", "r")) )
   {
   printf("Error open input file\n");
   return(0);
   }
 
for ( flines = 0 ; fgets(buf, sizeof(buf), fp) ; ++flines)
    ;
 
if ( flines == 0 )
   {
   printf("The file is empty\n");
   return(0);
   }
 
rline = rand() % (flines+1);
 
rewind(fp);
 
for ( i = 0 ; i < rline  ; ++i )
    fgets(buf, sizeof(buf), fp);
printf("%s", buf);
 
printf("Press any key to continue....\n");
getchar();
return(0);           
}
