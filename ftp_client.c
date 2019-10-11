#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#include "string.h"

void error (char *message) {
    perror(message);
    exit(0);
}

int main()
{
    int sockfd; //stores values returned by the socket system
    int portno; //stores the port number on which the server accepts connection
    int n; //return value for the read() and write() calls

    struct sockaddr_in serv_addr; //contains server address of the server to be connected to
    struct hostent *server; //holds host information in struct hostent inside netdb.h

    char buffer[256]; //sets buffer to read into

    char inputSize = 20;
    int option;
    char *command;
    char *hostName;
    char *portNumber;
    int quit = 0;

    FILE *fp;
    FILE *file;
    char *filename;

    //user enters port number, if port isnt available send error
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    //gets hostname from hostent in host ent *h_addr contains host ip address
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //sets all the fields in serv_addr to the values of the server
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    //attempts to connect to the given socket and returns error if it cannot connect
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting")


    while (quit != 1) {
        printf("Please enter a command\n");
        getline(&command, &inputSize, stdin);

        //drops string to lowercase
        for (int i = 0; i < strlen(command); i++) {
            command[i] = tolower(command[i]);
        }

        //converts input text to option paramater
        option = strcmp(command, "connect\n") == 0 ? 0 : strcmp(command, "list\n") == 0 ? 1 :
                strcmp(command, "retrieve\n") == 0 ? 2 : strcmp(command, "store\n") == 0 ? 3 :
                strcmp(command, "quit\n") == 0 ? 4 : -1;

        switch (option) {
            case 0: //connect
                printf("please enter a host name: \n");
                getline(&hostName, &inputSize, stdin);
                printf("please enter a port number: \n");
                getline(&portno, &inputSize, stdin);
                if (hostName = NULL) {
                    fprintf(stderr,"usage %s hostname port\n", hostName);
                    exit(0);
                }
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd < 0)
                    error("ERROR opening socket");

                server = gethostbyname(hostName);
                if (server == NULL) {
                    fprintf(stderr,"ERROR, no such host\n");
                    exit(0);
              
                bzero((char *) &serv_addr, sizeof(serv_addr));
                serv_addr.sin_family = AF_INET;
                bcopy((char *)server->h_addr,
                      (char *)&serv_addr.sin_addr.s_addr,
                      server->h_length);
                serv_addr.sin_port = htons(portno);

                //attempts to connect to the given socket and returns error if it cannot connect
            case 2: //retrieve
                printf("retrieve\n");
                n = read(sockfd, buffer, 255);
                fp = fopen("returned.txt","w");
                fprintf(fp,"%s",buffer);
                printf("created file returned.txt\n")
                break;
                
	        case 3: //store
                printf("store\n");
                printf("Enter file name:  \n"); //retrieve filename
                scanf("%c",&filename);
                file = fopen(filename, "r");
                fscanf(file,"%s",buffer); //read file from buffer
                n = write(sockfd, buffer, 255); //write file to socket
                if (n<0){
                    error("ERROR READING FROM SOCKET");
                }
                else {
                    printf("file sent!\n");
                }
                break;
            case 4: //quit
                printf("quit\n");
                quit = 1;
                break;
            default:
                error("incorrect command entered\n");
                break;

        }
    }
}
