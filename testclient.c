#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    // Declare initial vars
   																// file descriptor for our socket
    int portno = 9479;//needs to change																// server port to connect to
    int n = -1;																		// utility variable - for monitoring reading/writing from/to the socket
    char buffer[256];															// char array to store data going to and coming from the server
    struct sockaddr_in serverAddressInfo;						// Super-special secret C struct that holds address info for building our socket
    struct hostent *serverIPAddress;									// Super-special secret C struct that holds info about a machine's address
    
    
    
    // If the user didn't enter enough arguments, complain and exit
    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    /** If the user gave enough arguments, try to use them to get a port number and address **/
    
    // convert the text representation of the port number given by the user to an int
    portno = atoi(argv[2]);
    
    // look up the IP address that matches up with the name given - the name given might
    //    BE an IP address, which is fine, and store it in the 'serverIPAddress' struct
    
    int netinit=netserverinit(argv[1]);
    //set ERRNO

    int netfd = netopen("file.txt",O_RDWR);   
   
    
    // zero out the message buffer
    bzero(buffer,256);
    
    // get a message from the client
    fgets(buffer,255,stdin);
    
    // try to write it out to the server
    n = write(sockfd,buffer,strlen(buffer));
    
    // if we couldn't write to the server for some reason, complain and exit
    if (n < 0)
    {
        error("ERROR writing to socket");
    }
    
    // sent message to the server, zero the buffer back out to read the server's response
    bzero(buffer,256);
    
    // read a message from the server into the buffer
    n = read(sockfd,buffer,255);
    
    // if we couldn't read from the server for some reason, complain and exit
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    
    // print out server's message
    printf("%s\n",buffer);
    
    
    return 0;
}
