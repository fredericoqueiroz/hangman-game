#include "tcpClientUtility.h"

int setupClientSocket(const char *host, const char *service){

    // Define the criteria for address match
    struct addrinfo addrCriteria; // criteria for address match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // empty struct
    addrCriteria.ai_family = AF_UNSPEC;     // v4 or v6
    addrCriteria.ai_socktype = SOCK_STREAM; // stream socket
    addrCriteria.ai_protocol = IPPROTO_TCP; // TCP protocol

    // Get address
    struct addrinfo *servAddr; // holder for returned list of server addrs
    int rtnAddrInfo = getaddrinfo(host, service, &addrCriteria, &servAddr);
    if(rtnAddrInfo != 0)
        dieWithMessage(__FILE__, __LINE__, "error: getaddrinfo(): %s", gai_strerror(rtnAddrInfo));

    int clientSocket = -1;
    for(struct addrinfo *addr = servAddr; addr != NULL; addr->ai_next){
        // Create TCP stream socket
        clientSocket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if(clientSocket < 0)
            continue; // try next address
        
        // Establish the connection to the server
        if(connect(clientSocket, addr->ai_addr, addr->ai_addrlen) == 0)
            break; // connection suceeded; break and return socket

        close(clientSocket); // connection failes; try next addres
        clientSocket = -1;
    }

    freeaddrinfo(servAddr);
    return clientSocket;
}