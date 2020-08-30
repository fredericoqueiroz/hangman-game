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

/* 
void receiveServerMessage(int streamSocket, Message *message){
    //memset(message, 0, sizeof(Message));
    if(recv(streamSocket, message, sizeof(message), 0) != sizeof(message))
        dieWithMessage(__FILE__, __LINE__, "error: recv(): %s",strerror(errno));
}

void sendClientMessage(int streamSocket, Message *message){
    if(send(streamSocket, message, sizeof(Message), 0) != sizeof(Message))
        dieWithMessage(__FILE__, __LINE__, "error: send(): %s",strerror(errno));
}
 */

void handleClientGame(int networkSocket){

    Message message; // Create the game message struct
    memset(&message, 0, sizeof(message)); // empty struct

    // Receiving message 1
    //receiveServerMessage(networkSocket, &message);
    receiveMessage(networkSocket, &message);
    printMessage(message);

    //memset(&message, 0, sizeof(message));
    char guess = 'c';
    message.messageType = 2;
    message.guessedLetter = (uint8_t) guess;

    //sendClientMessage(networkSocket, &message);
    sendMessage(networkSocket, &message);
    printMessage(message);
/* 
    while (message.messageType != END_GAME_TYPE)
    {
        memset(&message, 0, sizeof(message)); // empty struct
        char guess;
        scanf("%c", &guess); // get guessed letter 

        message.guessedLetter = (uint8_t) guess;
        message.messageType = GUESS_TYPE;

        printMessage(message);
        sendClientMessage(networkSocket, &message);

        //memset(&message, 0, sizeof(message)); // empty struct
        receiveServerMessage(networkSocket, &message);
        printMessage(message);
    }
     */

    // ReadMessage()
    // ProcessMessage()

    // game ends
}

