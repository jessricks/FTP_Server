#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include "string.h"

void verify(int argv, int sockfd);
void createSocket(int sockfd, struct sockaddr_in serv_addr,int *portno, char *argv);
void bindSocket (int sockfd, struct sockaddr_in serv_addr);
void readFromSocket (char* buffer, int newsockfd, int n );
void error (char *message) {
    perror(message);
    exit(1);
}
int main(int argc, char *argv[]) { //argc is the port no.
    int sockfd, newsockfd; //store the values returned by the socket system
    int portno; //stores the port number on which the server accepts connection
    int clilen; // stores the size of the address of the client
    int n; //return value for the read() and write() calls

    struct sockaddr_in serv_addr, cli_addr; //struct inside <netinet/in.h> containse internet adress
    char buffer[256];

    //verifies inputs
    verify(argv, sockfd);

    //sets buffer to empty string
    bzero((char *) &serv_addr, sizeof(serv_addr));

    //sets port number from cmd line arguments
    portno = atoi(argv[1]);

    //creates socket for communication
    createSocket(sockfd, serv_addr, portno, argv);

    //bind socket to address
    bindSocket(sockfd, serv_addr);

    //all variables set inside of struct from netinet class sockaddr_in
    serv_addr.sin_family = AF_INET; // contains the code for the address family
    serv_addr.sin_addr.s_addr = INADDR_ANY; //contains the port number
    serv_addr.sin_port = htons(portno);

    //allows the server to listen for connections 5 maximum
    listen(sockfd,5);

    // attempts to accept server and holds until it has accepted
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("ERROR on accept");
    }

    while(1) {
    readFromSocket(buffer, newsockfd, n);
    }

}

    void verify(int argc, int sockfd){
        if (argc < 2) {
            error("ERROR, no port provided\n");
            exit(1);
        }

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0){
            error("ERROR opening socket");
            exit(1);
        }
    }

    void createSocket(int sockfd, struct sockaddr_in serv_addr, int *portno, char *argv) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("ERROR opening socket");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        *portno = atoi(argv[1]);
}

    void bindSocket (int sockfd, struct sockaddr_in serv_addr) {
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
            error("ERROR on binding");
}

void readFromSocket (char* buffer, int newsockfd, int n ) {
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);
}