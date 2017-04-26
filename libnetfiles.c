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

int sockfd;

int netserverinit(char * hostname){
	serverIPAddress = gethostbyname(hostname);

    if (serverIPAddress == NULL){
        fprintf(stderr,"ERROR, no such host\n");
        errno=HOST_NOT_FOUND;
        return -1;
    }
		return 0;
 }
    
/*
 1. try to build a socket .. if it doesn't work, complain and exit
 
 2. We now have the IP address and port to connect to on the server, we have to get that information into C's special address struct for connecting sockets
 
 3. zero out the socket address info struct .. always initialize!
 
 4. set a flag to indicate the type of network address we'll be using
 
 5. set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
 
 6. do a raw copy of the bytes that represent the server's IP address in the 'serverIPAddress' struct into our serverIPAddressInfo struct
 
 7. We now have a blank socket and a fully parameterized address info struct .. time to connect
 
 8. try to connect to the server using our blank socket and the address info struct if it doesn't work, complain and exit

 */

int netopen(const char *pathname, int flags){
	sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    // 1
    if (sockfd < 0){
        error("ERROR, creating socket");
    }
    struct sockaddr_in serverAddressInfo;
    // 2
    
    // 3
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
    
    // 4
    serverAddressInfo.sin_family = AF_INET;
    
    int portno = PORT;

    // 5
    serverAddressInfo.sin_port = htons(portno);
    
    // 6
    bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);
    // 7
    
    // 8
    if (connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) < 0){
        error("ERROR connecting");
    }

    printf("Connected\n");
	
    // char confirm[15];
    char buffer[256];
    strcpy(buffer,"o:");
    strcat(buffer,pathname);
    strcat(buffer,":");
    char * flagsC=malloc(sizeof(char)*10);
    sprintf(flagsC,"%d",flags);
    strcat(buffer,flagsC);
    int num=write(sockfd,buffer,strlen(buffer));
    if(num<0){
    	error("ERROR writing to socket");
    }

    bzero(buffer,256);
    num=read(sockfd,buffer,255);
	//printf("file des: %s",buffer);
    if(num==-1){
    	error("Error");
    }

    if(atoi(buffer)==1){
    	printf("Error");
    	exit(1);
    }
    ssize_t fd=(ssize_t)atoi(buffer);
    num=close(sockfd);
    if(num<0){
    	error("ERROR socket close was unsuccessful");
    }
// printf("Socket fd open %d\n",Socket->sockfd);
    return fd;

}

ssize_t netread(int fildes, void *buf, size_t nbyte){
	sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    // 1
    if (sockfd < 0){
        error("ERROR, creating socket");
    }
    struct sockaddr_in serverAddressInfo;
    // 2
    
    // 3
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
    
    // 4
    serverAddressInfo.sin_family = AF_INET;
    
    int portno = PORT;

    // 5
    serverAddressInfo.sin_port = htons(portno);
    
    // 6
    bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);
    // 7
    
    // 8
    if (connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) < 0){
        error("ERROR connecting");
    }

    char * nbyteStr=malloc((int)nbyte);
	sprintf(nbyteStr,"%d",(int)nbyte);
	char * fdc=malloc(10);
	fildes=fildes*(-1);
	sprintf(fdc,"%d",fildes);
	char * message=malloc(sizeof(char)*(strlen(nbyteStr)+2+strlen(fdc)));
	buf=(char*)malloc(sizeof(char)*256);
	strcat(message,fdc);
	strcat(message,nbyteStr);

	int messlength=strlen(message);
	char * messlc=malloc(messlength);
	sprintf(messlc,"%d",messlength);
	buf=malloc(sizeof(char)*(strlen(messlc)+3+strlen(message)));
	strcpy(buf,"r");
	strcat(buf, "?");
	strcat(buf,messlc);
	strcat(buf,"?");
	strcat(buf,message);
	int n=strlen(buf);
	int num=0;
	while(n>0){
		if(n>256){
			num=write(sockfd,buf,256);
			buf=&buf[256];
		}
		else{
			num=write(sockfd,buf,strlen(buf));
		}
	}
	num=close(sockfd);
	return 0;

}

ssize_t netwrite(int fildes, const void *buf, size_t nbyte){
	sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    // 1
    if (sockfd < 0){
        error("ERROR, creating socket");
    }
    struct sockaddr_in serverAddressInfo;
    // 2
    
    // 3
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
    
    // 4
    serverAddressInfo.sin_family = AF_INET;
    
    int portno = PORT;

    // 5
    serverAddressInfo.sin_port = htons(portno);
    
    // 6
    bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);
    // 7
    
    // 8
    if (connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) < 0){
        error("ERROR connecting");
    }
	char * message=malloc((int)nbyte+3);

	strcpy(message,"w");
	fildes=fildes*(-1);
	//how to know how much to malloc?
	char * fdc=malloc(10);
	sprintf(fdc,"%d",fildes);
	strcat(message,fdc);
	char * nbyteStr=malloc((int)nbyte);
	sprintf(nbyteStr,"%d",(int)nbyte);
	strcat(message,(char*)buf);
	int messLength=strlen(message);
	char * messlc=malloc(messLength);
	sprintf(messlc,"%d",messLength);
	char * sendbuf=malloc((int)nbyte+5+strlen(message));
	strcpy(sendbuf, "?");
	strcat(sendbuf,messlc);
	strcat(sendbuf,"?");
	strcat(sendbuf,message);
	int num=write(sockfd,buf,strlen(buf));
	num=close(sockfd);
	return 0;

}

 int netclose(int fd){
 	sockfd = -1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    // 1
    if (sockfd < 0){
        error("ERROR, creating socket");
    }
    struct sockaddr_in serverAddressInfo;
    // 2
    
    // 3
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
    
    // 4
    serverAddressInfo.sin_family = AF_INET;
    
    int portno = PORT;

    // 5
    serverAddressInfo.sin_port = htons(portno);
    
    // 6
    bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);
    // 7
    
    // 8
    if (connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) < 0){
        error("ERROR connecting");
    }
    //printf("hello\n");
    char buffer[256];
 	strcpy(buffer,"c");
 	char * fdc=malloc(10);
	sprintf(fdc,"%d",fd);
 	strcat(buffer,fdc);
 	int num=write(sockfd,buffer,strlen(buffer));
 	if(num<0){
    	error("ERROR writing to socket");
    }
    bzero(buffer,256);
    num=read(sockfd,buffer,255);
    int ret=atoi(buffer);
    if(num<0){
    	error("ERROR reading to socket");
    }
    num=close(sockfd);
 	return ret;
 }
