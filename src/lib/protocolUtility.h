#ifndef PROTOCOL_UTILITY_H_
#define PROTOCOL_UTILITY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_VERSION AF_INET
// AF_INET : IPv4
// AF_INET6: IPv6

enum sizeConstants {
  MAX_OCCURRENCES = 256,
  MAX_REQUESTS = 5
};

enum messageTypes {
  BEGIN_GAME_TYPE = 1,
  GUESS_TYPE = 2,
  ANSWER_TYPE = 3,
  END_GAME_TYPE = 4
};

typedef struct MessageInfo
{
  u_int8_t messageType; // Messages {1,2,3,4}
  u_int8_t wordSize; // Message {1}
  u_int8_t guessedLetter; // Message {2}
  u_int8_t occurrencesNumber; // Message {3}
  u_int8_t occurrencesPosition[MAX_OCCURRENCES]; // Message [3]
} Message;


void printMessage(Message message);

void receiveMessage(int streamSocket, Message *message);

void sendMessage(int streamSocket, Message *message);

//For debugging
void printSocketAddress(const struct sockaddr *address, FILE *stream);

//Handle erro with user message
void dieWithMessage(const char * file_name,int line_number, const char * format, ...);

#endif