#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "lib/protocolUtility.h"

int main(int argc, char **argv){

    // Check parameter
    if(argc != 2)
        dieWithMessage(__FILE__, __LINE__, "Sintaxe de Uso: %s <Server Port>", argv[0]);

    in_port_t serverPort = atoi(argv[1]);

    char word[256] = "familia";

    //Create socket for incoming connections
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket < 0)
        dieWithMessage(__FILE__, __LINE__, "error: socket(): %s",strerror(errno));

    // Create local address structure
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    //Bind to the local address
    if(bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0)
        dieWithMessage(__FILE__, __LINE__, "error: bind(): %s",strerror(errno));

    printf("Server Socket running on Port %d\n", ntohs(serverAddress.sin_port));

    //Listen for incoming connections
    if(listen(serverSocket, 5) < 0)
        dieWithMessage(__FILE__, __LINE__, "error: listen(): %s",strerror(errno));

    //int messageBuffer[BUFSIZE];

    while(1){ //Run forever

        struct sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);

        printf("Wainting for client to connect...\n");
        // Wait for client to connect
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLen);
        if(clientSocket < 0)
            dieWithMessage(__FILE__, __LINE__, "error: accept(): %s",strerror(errno));

        //CONECTOU A UM CLIENTE!
        char clientName[INET_ADDRSTRLEN]; 
        if(inet_ntop(AF_INET, &clientAddress.sin_addr.s_addr, clientName, sizeof(clientName)) != NULL)
            printf("Handling client %s/%d\n", clientName, ntohs(clientAddress.sin_port));
        else
            fprintf(stderr, "Unable to get cliente address.\n");

        //HANDLE THE GAME WITH CLIENT
        handleServerGame(clientSocket, word);
        close(clientSocket);
    }
    //close(serverSocket);
    return 0;
}