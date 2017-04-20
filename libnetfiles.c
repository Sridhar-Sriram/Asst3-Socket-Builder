#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#define PORT 9479
#define IP hostname

void error(char *msg)
{
    perror(msg);
    exit(0);
}

struct hostent *serverIPAddress;

int netserverinit(char * hostname){
	printf("hostname %s\n",hostname);
	serverIPAddress = gethostbyname(hostname);

    if (serverIPAddress == NULL){
        fprintf(stderr,"ERROR, no such host\n");
        errno=HOST_NOT_FOUND;
        return -1;
    }

    return 0;
}

int netopen(const char *pathname, int flags){

	int sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// try to build a socket .. if it doesn't work, complain and exit
    if (sockfd < 0)
    {
        error("ERROR, creating socket");
    }
    struct sockaddr_in serverAddressInfo;
        /** We now have the IP address and port to connect to on the server, we have to get    **/
    /**   that information into C's special address struct for connecting sockets                     **/
    
    // zero out the socket address info struct .. always initialize!
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
    
    // set a flag to indicate the type of network address we'll be using
    serverAddressInfo.sin_family = AF_INET;
    
    int portno = PORT;

    // set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
    serverAddressInfo.sin_port = htons(portno);
    
    // do a raw copy of the bytes that represent the server's IP address in
    //   the 'serverIPAddress' struct into our serverIPAddressInfo struct
    bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);
    /** We now have a blank socket and a fully parameterized address info struct .. time to connect **/
    
    // try to connect to the server using our blank socket and the address info struct
    //   if it doesn't work, complain and exit

    if (connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) < 0)
    {
        error("ERROR connecting");
    }
    printf("Socket has connected.\n");
    char buffer[256];
    strcpy(buffer,"Hello");
    send(sockfd,buffer,strlen(buffer),0);

     //printf("Please enter the message: ");
    return 0;

}

ssize_t netread(int fildes, void *buf, size_t nbyte){
	

}

ssize_t netwrite(int fildes, const void *buf, size_t nbyte){

}

 int netclose(int fd){

 }