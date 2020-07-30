#include "tcpServerUtility.h"
#include "protocolUtility.h"

int setupServerSocket(const char *service){

    // Define the criteria for address match
    struct addrinfo addrCriteria; // criteria for address match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // empty struct
    addrCriteria.ai_family = AF_UNSPEC; // v4 or v6
    addrCriteria.ai_flags = AI_PASSIVE; // any address/port
    addrCriteria.ai_socktype = SOCK_STREAM; // stream socket
    addrCriteria.ai_protocol = IPPROTO_TCP; // TCP protocol

    // Get address
    struct addrinfo *servAddr; // holder for returned list of server addrs
    int rtnAddrInfo = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
    if(rtnAddrInfo != 0)
        dieWithMessage(__FILE__, __LINE__, "error: getaddrinfo(): %s", gai_strerror(rtnAddrInfo));

    int serverSocket = -1;
    for(struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next){
        // Create TCP stream socket
        serverSocket = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol);
        if(serverSocket < 0)
            continue; // try next address

        // Bind to the local address and set socket to list
        if((bind(serverSocket, servAddr->ai_addr, servAddr->ai_addrlen) == 0) &&
           (listen(serverSocket, MAX_REQUESTS) == 0)){
            // Print socket local address
            struct sockaddr_storage localAddr;
            socklen_t addrSize = sizeof(localAddr);
            if(getsockname(serverSocket, (struct sockaddr *) &localAddr, &addrSize) < 0)
                dieWithMessage(__FILE__, __LINE__, "error: getsockname(): %s",strerror(errno));
            
            fputs("Binding to ", stdout);
            PrintSocketAddress((struct sockaddr *) &localAddr, stdout);
            fputc('\n', stdout);
            break;
        }

        close(serverSocket); // close and try again
        serverSocket = -1;
    }

    freeaddrinfo(servAddr);
    return serverSocket;
}

int acceptClientConnection(int serverSocket){
    struct sockaddr_storage clntAddr; // client address
    socklen_t clntAddrLen = sizeof(clntAddr);

    // wait for client to connect
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if(clientSocket < 0)
        dieWithMessage(__FILE__, __LINE__, "error: accept(): %s",strerror(errno));

    // clientSocket is connect to a client
    fputs("Handling client ", stdout);
    PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);
    fputc('\n', stdout);

    return clientSocket;
}