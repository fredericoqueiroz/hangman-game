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

enum sizeConstants {
  BUFSIZE = 130,
  MAX_OCCURRENCES = 255,
  MAX_REQUESTS = 5
};

typedef struct MessageInfo
{
  u_int8_t messageType; // Messages {1,2,3,4}
  u_int8_t wordSize; // Message {1}
  u_int8_t guessedLetter; // Message {2}
  u_int8_t occurrencesNumber; // Message {3}
  u_int8_t occurrencesPosition[MAX_OCCURRENCES]; // Message [3]
} Message;

//Handle erro with user message
void dieWithMessage(const char * file_name,int line_number, const char * format, ...);

//void handleServerGame(int clientSocket, char * word);

//For debugging
void printSocketAddress(const struct sockaddr *address, FILE *stream);

void printMessage(Message message);

void receiveMessage(int streamSocket, Message *message);

void sendMessage(int streamSocket, Message *message);

#endif