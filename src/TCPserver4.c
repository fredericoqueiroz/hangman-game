#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
    
    if(argc != 2){
        fputs("Erro de sistaxe de uso\n", stderr);
        exit(1);
    }

    in_port_t serverPort = atoi(argv[1]);

    char server_message[256] = "You have a missed call from server";

    //Create socket for incoming connections
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_socket < 0){
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

    //Bind to the local address
    if(bind(server_socket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0){
        fputs("Erro in bind()", stderr);
        exit(1);
    }

    //Listen for incoming connections
    if(listen(server_socket, 5) < 0){
        fputs("Erro in listen()", stderr);
        exit(1);
    }

    int cliente_socket = accept(server_socket, NULL, NULL);

    send(cliente_socket, server_message, sizeof(server_message), 0);

    close(server_socket);
    return 0;
}