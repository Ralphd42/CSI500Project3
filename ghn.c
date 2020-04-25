
/*
    Helper file to get hostname

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void) {

    char hs[1024];
    gethostname(hs, 1024);
    puts(hs);

    return EXIT_SUCCESS;
}