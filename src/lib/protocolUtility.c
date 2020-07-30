#include "protocolUtility.h"

void dieWithMessage(const char * file_name,int line_number, const char * format, ...){
    va_list vargs;
    va_start(vargs, format);
    fprintf(stderr, "%s:%d: ",file_name, line_number);
    vfprintf(stderr, format, vargs);
    fprintf (stderr, ".\n");
    va_end (vargs);
    exit (EXIT_FAILURE);
}

void clearBuffer(int * buffer, const int bufferSize){
    int i;
    for(i=0; i<bufferSize; i++)
        buffer[i] = 0;
}

void handleGuessMessage(int clientSocket, char * word, int * buffer){
    int wordSize = strlen(word);
    int i, j, occurrences = 0;
    char guess = buffer[1];

    //memset(&buffer, 0, sizeof(buffer));
    clearBuffer(buffer, BUFSIZE);
    j = 2;
    for(i=0; i<wordSize; i++){
        if(guess == word[i]){
            buffer[j] = i;
            j++;
            occurrences++;
        }
    }
    buffer[0] = 3;
    buffer[1] = occurrences;

    printf("Buffer:");
    for(i=0; i<occurrences+2; i++){
        printf(" %d", buffer[i]);
    }
    printf("\n");

    //send(clientSocket, buffer, sizeof(buffer), 0);

}

void handleServerGame(int clientSocket, char * word){
    int buffer[BUFSIZE];
    clearBuffer(buffer, BUFSIZE);
    //memset(&buffer, -1, sizeof(buffer));
    printf("Last position buffer %d\n", buffer[BUFSIZE-1]);
    //Sending the begin game message (type 1)
    buffer[0] = 1;
    buffer[1] = strlen(word);
    //ssize_t numBytesSend = send(clientSocket, buffer, sizeof(buffer), 0);
    send(clientSocket, buffer, sizeof(buffer), 0);

    //printf("Word: %s %d\n", word, buffer[1]);
    //Handle the guess and result messages (type 2 and type 3)/    
    while(1){ // ENQUANTO A PALAVRA NAO FOI ENCONTRADA
        //memset(&buffer, -1, sizeof(buffer));
        clearBuffer(buffer, BUFSIZE);
        recv(clientSocket, buffer, sizeof(buffer), 0);
        printf("Guess Recived (Type | Guess): %d | %d\n", buffer[0], buffer[1]);

        handleGuessMessage(clientSocket, word, buffer);

        
    }
}

void PrintSocketAddress(const struct sockaddr *address, FILE *stream) {
  // Test for address and stream
  if (address == NULL || stream == NULL)
    return;

  void *numericAddress; // Pointer to binary address
  // Buffer to contain result (IPv6 sufficient to hold IPv4)
  char addrBuffer[INET6_ADDRSTRLEN];
  in_port_t port; // Port to print
  // Set pointer to address based on address family
  switch (address->sa_family) {
  case AF_INET:
    numericAddress = &((struct sockaddr_in *) address)->sin_addr;
    port = ntohs(((struct sockaddr_in *) address)->sin_port);
    break;
  case AF_INET6:
    numericAddress = &((struct sockaddr_in6 *) address)->sin6_addr;
    port = ntohs(((struct sockaddr_in6 *) address)->sin6_port);
    break;
  default:
    fputs("[unknown type]", stream);    // Unhandled type
    return;
  }
  // Convert binary to printable address
  if (inet_ntop(address->sa_family, numericAddress, addrBuffer,
      sizeof(addrBuffer)) == NULL)
    fputs("[invalid address]", stream); // Unable to convert
  else {
    fprintf(stream, "%s", addrBuffer);
    if (port != 0)                // Zero not valid in any socket addr
      fprintf(stream, "-%u", port);
  }
  
}