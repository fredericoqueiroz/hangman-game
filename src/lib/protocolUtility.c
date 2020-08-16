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

/* void clearBuffer(int * buffer, const int bufferSize){
    int i;
    for(i=0; i<bufferSize; i++)
        buffer[i] = 0;
} */

/* void handleGuessMessage(int clientSocket, char * word, int * buffer){
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

} */

/* void handleServerGame(int clientSocket, char * word){
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
} */

void printSocketAddress(const struct sockaddr *address, FILE *stream) {
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

void printMessage(Message message){
  fprintf(stdout, "Message Type: %d\n", message.messageType);
  switch (message.messageType){
  case 1:
    fprintf(stdout, "Message Type: %d\n", message.messageType);
    fprintf(stdout, "Word Size: %d\n", message.wordSize);
    break;
  case 2:
    fprintf(stdout, "Message Type: %d\n", message.messageType);
    fprintf(stdout, "Guessed Letter: %d\n", message.guessedLetter);
    break;
  case 3:
    fprintf(stdout, "Message Type: %d\n", message.messageType);
    fprintf(stdout, "Number of occurences:%d\n", message.occurrencesNumber);
    fprintf(stdout, "Positions:");
    for(int i=0; i < message.occurrencesNumber; i++)
      fprintf(stdout, " %d\n", message.occurrencesPosition[i]);
    fprintf(stdout, "\n");
    break;
  case 4:
    fprintf(stdout, "Message Type: %d\n", message.messageType);
  default:
    fprintf(stdout, "Invalid message type\n");
    break;
  }
}

void receiveMessage(int streamSocket, Message *message){
  FILE *stream = fdopen(streamSocket, "r");

  // read the message from the socket stream
/*   if(fread(message, sizeof(Message), 1, stream) != 1)
    dieWithMessage(__FILE__, __LINE__, "error: fread(): %s",strerror(errno)); */
  fread(message, sizeof(Message), 1, stream);

  // convert to host byte order
  message->messageType = message->messageType;
  message->wordSize = ntohs(message->wordSize);
  message->guessedLetter = ntohs(message->guessedLetter);
  message->occurrencesNumber = ntohs(message->occurrencesNumber);
  for(int i=0; i < message->occurrencesNumber; i++)
    message->occurrencesPosition[i] = ntohs(message->occurrencesPosition[i]);

  //fprintf(stdout, "Message Type: %d\n", message->messageType);

  fclose(stream);
}

void sendMessage(int streamSocket, Message *message){

  fprintf(stdout, "send2: Message Type: %d\n", message->messageType);

  // HTONS ESTAH ALTERANDO O messageType (remover htons)
  // convert to network byte order
  message->messageType = message->messageType;
  message->wordSize = htons(message->wordSize);
  message->guessedLetter = htons(message->guessedLetter);
  message->occurrencesNumber = htons(message->occurrencesNumber);
  for(int i=0; i < message->occurrencesNumber; i++)
    message->occurrencesPosition[i] = htons(message->occurrencesPosition[i]);

  fprintf(stdout, "send2: Message Type: %d\n", message->messageType);

  if(send(streamSocket, &message, sizeof(Message), 0) != sizeof(Message))
    dieWithMessage(__FILE__, __LINE__, "error: send(): %s",strerror(errno));
}
