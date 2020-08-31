#include "tcpServerUtility.h"

int setupServerSocket(const char *service){

    // Define the criteria for address match
    struct addrinfo addrCriteria; // criteria for address match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // empty struct
    addrCriteria.ai_family = IP_VERSION; // Defined in protocolUtility.h
    addrCriteria.ai_flags = AI_PASSIVE; // any address/port
    addrCriteria.ai_socktype = SOCK_STREAM; // stream socket
    addrCriteria.ai_protocol = IPPROTO_TCP; // TCP protocol

    // Get address
    struct addrinfo *servAddr;
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
            
            fprintf(stdout, "Binding to ");
            printSocketAddress((struct sockaddr *) &localAddr, stdout);
            fprintf(stdout,"\n");
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
    printSocketAddress((struct sockaddr *) &clntAddr, stdout);
    fputc('\n', stdout);

    return clientSocket;
}

void handleGuess(const char *word, uint8_t *mask, Message *message){

    // Clean occurrences
    message->occurrencesNumber = 0;

    for(int i=0; i < MAX_OCCURRENCES; i++)
        message->occurrencesPosition[i] = 0;

    // Check the guess
    for(int k=0; k < message->wordSize; k++){
        if(word[k] == message->guessedLetter){
            mask[k] = 1; // mark true all mask position corresponding to guess
            message->occurrencesPosition[message->occurrencesNumber] = k;
            message->occurrencesNumber += 1;
        }
    }

    //check if the game is over
    int gameover = 1;
    for(int j=0; j < message->wordSize; j++){
        if(mask[j] == 0){
            gameover = 0;
            break;
        }
    }

    //write the result in the message
    if(gameover)
        message->messageType = END_GAME_TYPE;
    else
        message->messageType = ANSWER_TYPE;
}

void handleServerGame(int clientSocket, const char *word){

    Message message; // Create the game message struct
    memset(&message, 0, sizeof(message)); // empty struct

    // Sending message 1
    message.messageType = BEGIN_GAME_TYPE;
    message.wordSize = strlen(word);
    sendMessage(clientSocket, &message);

    uint8_t mask[MAX_OCCURRENCES]; // Create mask to control the state of the game
    for(int i=0; i < MAX_OCCURRENCES; i++)
        mask[i] = 0;

    while (message.messageType != END_GAME_TYPE){
        // recieve guess (message 2)
        receiveMessage(clientSocket, &message);
        printMessage(message);

        handleGuess(word, mask, &message);

        sendMessage(clientSocket ,&message);
        //printMessage(message);
    }
    //game ends
}

