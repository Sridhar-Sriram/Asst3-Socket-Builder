#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "libnetfiles.c"
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    // Declare initial vars
   																// file descriptor for our socke															// server port to connect to
    int n = -1;																		// utility variable - for monitoring reading/writing from/to the socket
    															// char array to store data going to and coming from the server
    struct sockaddr_in serverAddressInfo;						// Super-special secret C struct that holds address info for building our socket
    struct hostent *serverIPAddress;									// Super-special secret C struct that holds info about a machine's address
    
    // If the user didn't enter enough arguments, complain and exit
    // if (argc < 3)
    // {
    //     fprintf(stderr,"usage %s hostname port\n", argv[0]);
    //     exit(0);
    // }
    
    /** If the user gave enough arguments, try to use them to get a port number and address **/
    
    // convert the text representation of the port number given by the user to an int
    
    // look up the IP address that matches up with the name given - the name given might
    //    BE an IP address, which is fine, and store it in the 'serverIPAddress' struct
    
    int netinit=netserverinit("127.0.0.1");
    int netfd = netopen("file.txt",O_RDONLY);   
    int netclo=netclose(netfd);
    
    return 0;
}
