/*
 simple client to work with server.c program.
 * Host name and port used by server is to be
 * passed as arguments.
 *
 * To test: Open a terminal window.
 * At prompt ($ is my prompt symbol) you may
 * type the following as a test:
 *
 * $./client 127.0.0.1 54554
 * Please enter the message: Operating Systems is fun!  
 * I got your message
 * $ 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
const char * UpOut  = "fileUpper.txt";
const char * ChrOut = "fileChar.txt" ;

void error(const char *msg);
int main( )
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
     
    portno = 5540;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("192.168.1.15");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)     &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
   // fgets(buffer,255,stdin);
    int readlen =0;
    char * inBuff ;
    size_t bufflen =0;
     
    FILE* fpw = fdopen(dup(sockfd),"w");
    if(fpw==NULL)
    {
        error("FAILED TO OPEN FD");
    }
    FILE *fp = fopen("./tkntst.c", "r");
    char c=getc(fp);
    while(c != EOF)
   {
       putc(c,fpw);
        c=getc(fp);
    }
    putc(EOF,fpw);
    fclose (fp);
    fclose(fpw);
    close(sockfd);
    return 0;
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}