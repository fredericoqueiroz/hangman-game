#ifndef SERVER_UTILITY_H_
#define SERVER_UTILITY_H_

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> // addrinfo
#include <errno.h>

int setupServerSocket(const char *service);

int acceptClientConnection(int serverSocket);

void handleServerGame(int serverSocket, const char *word);

#endif