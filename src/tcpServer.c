#include <stdio.h>
#include <unistd.h>
#include "lib/protocolUtility.h"
#include "lib/tcpServerUtility.h"

int main(int argc, char **argv){

    // Check parameter
    if(argc != 2)
        dieWithMessage(__FILE__, __LINE__, "Sintaxe de Uso: %s <Server Port>", argv[0]);

    //in_port_t service = atoi(argv[1]);
    char * service = argv[1];

    //Create socket for incoming connections
    int serverSocket = setupServerSocket(service);
    if(serverSocket < 0)
        dieWithMessage(__FILE__, __LINE__, "error: setupServerSocket(): %s",strerror(errno));

    for(;;){
        // Create new connection to client socket
        int clientSocket = acceptClientConnection(serverSocket);

        handleServerGame(clientSocket, "casa");

        close(clientSocket);
    }
}