#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
 
#include   <unistd.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include   <fcntl.h>
#include<ctype.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
 
 
#define pnum 5540
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int  socketInputD(int portno);

int main()
{
    char buffer[256];
    int fd =socketInputD(pnum);
    int c;
    int cnt =0;
    int n=0;
    /*do{
     bzero(buffer,256);
        n = read(fd,buffer,255);
    printf("|%s|%d ", buffer,n);
    fflush(stdout);
    cnt++;
    }while (n<0);
    */
    FILE * fin = fdopen(dup(fd),"r+");
    do {
        cnt++;
    c = getc(fin);
    if(c!=EOF){
    putchar (c); //do whatever you want with this character.
    }else{putchar('.');}
  } while (c != EOF  );
    
    
    
    fclose(fin);
    
    close(fd);


    printf("\nDONE\n");
    return 0;


}


int  socketInputD(int portno)
{
    int sockfd, newsockfd ;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
      error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, 
        (struct sockaddr *) &cli_addr, 
        &clilen);
    if (newsockfd < 0) 
         error("ERROR on accept");
    bzero(buffer,256);
   return newsockfd;
   // close(0);
    //dup(sockfd);
    //close(sockfd);
}//