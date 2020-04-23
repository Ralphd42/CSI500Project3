#include<stdio.h>
 
#include<string.h>
#include<stdlib.h>
#include    <ctype.h>

#include <stdbool.h>
int main()
{
    char * txt = "uhg@iugiujhiabig.string.ofAA.text";

    char * first = strchr(txt,'@');
    ++first;
    printf("@     - |%s|\n\n", first);
    char * outs  =strrchr ( first,'.');
    

    printf("\n%s",outs);
    printf( "\n%ld", (outs -first));
    printf("\n%s",(first +(outs -first)));
    char ip[outs-first+1];
    memset(ip, '\0', sizeof(ip));
    strncpy(ip,first, (outs -first)   );
    printf("\n%s\n", ip);
    printf("txt: %s", txt);
    return 0;
}