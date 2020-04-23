#include"socklib.h"

void redirectToSocket ( int portno, char * ServerAddress)
{
    int sockfd , n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
     
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(ServerAddress);
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
    close(1);
    dup(sockfd);
    close(sockfd);

}


/*
    this handles reading from a port
    takes port as integer
    returns void.


*/
void socketInput(int portno)
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
    close(0);
    dup(sockfd);
    close(sockfd);
}

