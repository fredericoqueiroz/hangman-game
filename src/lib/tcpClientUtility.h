#ifndef CLIENT_UTILITY_H_
#define CLIENT_UTILITY_H_

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> // addrinfo
#include "protocolUtility.h"

int setupClientSocket(const char *host, const char *service);

void handleClientGame(int clientSocket);

#endif