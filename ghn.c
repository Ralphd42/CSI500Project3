
/*
    Helper file to get hostname

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
int main(void) {
    struct ifaddrs *id;
     struct hostent *host_entry;
    char hs[1024];
    int hn =gethostname(hs, 1024);
    puts(hs);
    char *IPbuffer;
    host_entry = gethostbyname(hs);
    IPbuffer = inet_ntoa(*((struct in_addr*) 
                           host_entry->h_addr_list[0])); 
    //puts(IPbuffer);
     struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;

    getifaddrs (&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family==AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);
        }
    }

    freeifaddrs(ifap);
     
    return EXIT_SUCCESS;
}