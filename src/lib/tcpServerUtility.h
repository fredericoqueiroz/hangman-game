#ifndef SERVER_UTILITY_H_
#define SERVER_UTILITY_H_

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> // addrinfo
#include <errno.h>
#include "protocolUtility.h"

int setupServerSocket(const char *service);

int acceptClientConnection(int serverSocket);

void handleGuess(const char *word, uint8_t *mask, Message *message);

void handleServerGame(int clientSocket, const char *word);

#endif