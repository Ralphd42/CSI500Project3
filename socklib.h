#ifndef __SOCKLIB_H__
#define __SOCKLIB_H__
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "shellcmd.h"


void socketInput(int portno);
void redirectToSocket ( int portno, char * ServerAddress);









#endif



