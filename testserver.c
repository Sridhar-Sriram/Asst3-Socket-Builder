/* A simple server in the internet domain using TCP
 The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

//create struct with any other data you need for clientServiceThread

int serverOpen(char * buffer){
    int n=2;
    char * pathname=malloc(sizeof(char)*strlen(buffer));
    char * flag=malloc(sizeof(char)*2);
    while(n<strlen(buffer)){
        if(buffer[n]==':'){
            memcpy(pathname,&buffer[2],n-2);
            //printf("pathname: %s\n",pathname);
            memcpy(flag,&buffer[n+1],strlen(buffer)-n);
            //printf("flag: %s\n",flag);
        }
        n++;
    }
    int fd=open(pathname,atoi(flag));
   // printf("fd %d\n",fd);
    return fd;
}

int serverClose(char * buffer){
    char * fdc=malloc(sizeof(char)*10);
    memcpy(fdc,&buffer[1],strlen(buffer)-1);
    int fd=atoi(fdc);
    fd=(-1)*fd;
    int c=close(fd);
    printf("c %d\n",c);
    return c;
}

void * clientServiceThread(void * clientSocket){
    //read from the socket

    enum type {OPEN, WRITE, READ, CLOSE};
    int leave=0;
    while(leave==0){
        enum type whatNext;
        char buffer[256];
        bzero(buffer,256);
        int num=read(*(int*)clientSocket,buffer,255);
        if(num<0){
            error("ERROR reading from socket");
        }

        if(strncmp(buffer,"o",1)==0){
            int fd=serverOpen(buffer);
            char fdc[10];
            fd=(-1)*fd;
            sprintf(fdc,"%d",fd);
            num=write(*(int*)clientSocket,fdc,strlen(fdc));
        }
        else if(strcmp(buffer,"w")==0){
            whatNext=WRITE;
        }
        else if(strcmp(buffer,"r")==0){
            whatNext=READ;
        }
        else if(strncmp(buffer,"c",1)==0){
            char fdclose[10];
            int sclose=serverClose(buffer);
            sprintf(fdclose,"%d",sclose);
            num=write(*(int*)clientSocket,fdclose,strlen(fdclose));
        }
        leave=1;
    //     bzero(buffer,256);
    //     num=read(*(int*)clientSocket,buffer,255);
    //     int totalBytes=atoi(buffer);
    //     char * message=malloc(sizeof(char)*totalBytes);
    //     int bytesRec=0;
    //     printf("buffer %s\n",buffer);
    //     bzero(buffer,256);
    //     while(bytesRec<totalBytes){
    //         num=read(*(int*)clientSocket,buffer,255);
    //         if(message==NULL){
    //             strcpy(message,buffer);
    //         }
    //         else{
    //             strcat(message,buffer);
    //         }
    //         bytesRec+=num;
    //     }
    //     printf("buffer %s\n",buffer);
    //     int n=0;
    //     char * token=NULL;
    //     char * flag=malloc(sizeof(char)*3);
    //     while(n<totalBytes){
    //         if(message[n]==':'){
    //             token=malloc(sizeof(char)*(n+1));
    //             memcpy(token, message, n);
    //             flag=&message[n+1];
    //         }
    //     }
    //     //printf("token %s\n",token);
    //     if(whatNext==OPEN){
    //         int fd=open(message,atoi(flag));
    //         char fdc[10];
    //         sprintf(fdc,"%d",fd);
    //         num=write(*(int*)clientSocket,fdc,strlen(fdc));
    //     }
   }
    return NULL;
}

int main(int argc, char *argv[])
{   
    int sockfd = -1;														
    int newsockfd = -1;												
    int portno = 9479;														
    int clilen = -1;															
    int n = -1;																
    char buffer[256];													
    
    struct sockaddr_in serverAddressInfo;	// Super-special secret C struct that holds address info for building our server socket
    struct sockaddr_in clientAddressInfo;	// Super-special secret C struct that holds address info about our client socket
    // If the user didn't enter enough arguments, complain and exit
    /** If the user gave enough arguments, try to use them to get a port number and address **/
    
    // convert the text representation of the port number given by the user to an int
    
    // try to build a socket .. if it doesn't work, complain and exit
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    /** We now have the port to build our server socket on .. time to set up the address struct **/
    
    // zero out the socket address info struct .. always initialize!
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
    
    // set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
    serverAddressInfo.sin_port = htons(portno);
    
    // set a flag to indicate the type of network address we'll be using
    serverAddressInfo.sin_family = AF_INET;
    
    // set a flag to indicate the type of network address we'll be willing to accept connections from
    serverAddressInfo.sin_addr.s_addr = INADDR_ANY;
    
    /** We have an address struct and a socket .. time to build up the server socket **/
    
    // bind the server socket to a specific local port, so the client has a target to connect to
    if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0)
    {
        error("ERROR on binding");
    }
    
    // set up the server socket to listen for client connections
    listen(sockfd,5);
    
    while(1){
        // determine the size of a clientAddressInfo struct
        clilen = sizeof(clientAddressInfo);
        
        // block until a client connects, when it does, create a client socket
        newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, (socklen_t*)&clilen);
        
        /** If we're here, a client tried to connect **/
        // if the connection blew up for some reason, complain and exit
        if (newsockfd < 0)
        {
            error("ERROR on accept");
        }
            //make socket array
        int *sockptr=malloc(sizeof(int));
        sockptr=&newsockfd;
        pthread_t threadptr;
        void* (*thread_proc)(void *)=clientServiceThread;
        int p=pthread_create(&threadptr,NULL,thread_proc,(void*)sockptr);
        if(p!=0){
            error("ERROR when creating thread.");
        }
        pthread_detach(threadptr);
            //close(newsockfd);
            //exit(1);
            // zero out the char buffer to receive a client message
        bzero(buffer,256);
            
            // try to read from the client socket
            //n = read(newsockfd,buffer,255);
            
            // if the read from the client blew up, complain and exit
           //  if (n < 0)
           //  {
           //      //printf("hello\n");
           //      error("ERROR reading from socket");
           //  }

           //  // try to write to the client socket
           // // n = write(newsockfd,"I got your message",18);
            
           //  // if the write to the client below up, complain and exit
           //  if (n < 0)
           //  {
           //      error("ERROR writing to socket");
           //  }
   }
    return 0; 
}
