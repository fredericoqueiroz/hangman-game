#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum sizeConstants {
  BUFSIZE = 128
};

int main(int argc, char **argv){
    
    if(argc != 2){
        fputs("Erro de sistaxe de uso\n", stderr);
        exit(1);
    }

    in_port_t serverPort = atoi(argv[1]);

    //char server_message[256] = "Hello from server\n";

    char word[256] = "familia";

    int begin_message[2];
    begin_message[0] = 1;
    begin_message[1] = strlen(word);

    //printf("word size: %d", begin_message[1])

    //Create socket for incoming connections
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket < 0){
        fputs("Erro in socket()", stderr);
        exit(1);
    }

    // Create local address structure
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    //serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Server Socket running on Port %d\n", ntohs(serverAddress.sin_port));

    //Bind to the local address
    if(bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0){
        fputs("Erro in bind()", stderr);
        exit(1);
    }

    //Listen for incoming connections
    if(listen(serverSocket, 5) < 0){
        fputs("Erro in listen()", stderr);
        exit(1);
    }

    int messageBuffer[BUFSIZE];

    while(1){ //Infinite loop

        memset(&messageBuffer, 0, sizeof(messageBuffer));
        struct sockaddr_in clientAddress; // Client Address
        // Set len of clientAddress structure
        socklen_t clientAddressLen = sizeof(clientAddress);

        printf("Wainting for client...\n");
        // Wait for client to connect
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLen);
        if(clientSocket < 0){
            fputs("Erro in accept()", stderr);
            exit(1);
        }

        //CONECTOU A UM CLIENTE!

        char clientName[INET_ADDRSTRLEN]; 
        if(inet_ntop(AF_INET, &clientAddress.sin_addr.s_addr, clientName, sizeof(clientName)) != NULL)
            printf("Handling client %s/%d\n", clientName, ntohs(clientAddress.sin_port));
        else
            fputs("Unable to get cliente address\n", stderr);

        // Sending message type 1
        ssize_t numBytesSend = send(clientSocket, begin_message, sizeof(begin_message), 0);

        printf("Bytes enviados na mensagem 1: %ld\n", numBytesSend);
        while(1){
            //send(clientSocket, server_message, sizeof(server_message), 0);
            recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);
            printf("Guess Recived (Type | Guess): %d | %d\n", messageBuffer[0], messageBuffer[1]);
        }
    }
    //close(serverSocket);
    return 0;
}