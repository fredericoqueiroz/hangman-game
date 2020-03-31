#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//socket related functions
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv){

    if(argc < 3){
        fputs("Erro de sistaxe de uso\n", stderr);
        exit(1);
    }

    char *serverIP = argv[1];
    in_port_t serverPort = atoi(argv[2]);

    // Create TCP stream socket
    int network_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(network_socket < 0){
        fputs("Erro in socket()", stderr);
        exit(1);
    }

    //Create the server address structure
    struct sockaddr_in serverAddress; // Server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET; // IPv4 address
    serverAddress.sin_port = htons(serverPort); // Server port
    
    //Convert address
    if(inet_pton(AF_INET, serverIP, &serverAddress.sin_addr.s_addr)<=0){
        fputs("Erro in inet_pton()", stderr);
        exit(1);
    }

    printf("Trying to connect...\n");
    // Establish the connection to the server
    if(connect(network_socket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
        fputs("Erro in connect()", stderr);
        exit(1);
    }
    printf("Connected...\n");


    //recieve data from the server
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response), 0);

    printf("Server data: %s\n", server_response);

    //closing socket
    close(network_socket);
    return 0;
}
