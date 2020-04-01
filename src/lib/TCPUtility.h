#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>

//Handle erro with user message
void dieWithMessage(const char * file_name,int line_number, const char * format, ...);

void handleServerGame(int clientSocket, char * word);

enum sizeConstants {
  BUFSIZE = 128
};


#endif