#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//socket related functions
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include "lib/protocolUtility.h"
#include "lib/tcpClientUtility.h"

int main(int argc, char **argv){

    if(argc != 3)
        dieWithMessage(__FILE__, __LINE__, "Sintaxe de Uso: %s <ip-servidor> <porta-servidor>", argv[0]);

    char *serverIP = argv[1];
    //in_port_t service = atoi(argv[2]);
    char *service = argv[2];

    // Setup a connected TCP sream socket
    int networkSocket = setupClientSocket(serverIP, service);
    if(networkSocket < 0)
        dieWithMessage(__FILE__, __LINE__, "error: setupClientSocket(): %s",strerror(errno));

    handleClientGame(networkSocket);

    close(networkSocket);
    return 0;
}