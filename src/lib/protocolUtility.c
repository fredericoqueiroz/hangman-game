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
  
  switch (message.messageType){
  case BEGIN_GAME_TYPE:
    fprintf(stdout, "Message Type: %d\n", message.messageType);
    fprintf(stdout, "Word Size: %d\n", message.wordSize);
    break;
  case GUESS_TYPE:
    fprintf(stdout, "Message Type: %d\n", message.messageType);
    fprintf(stdout, "Guessed Letter: %c\n", (char) message.guessedLetter);
    break;
  case ANSWER_TYPE:
    fprintf(stdout, "Message Type: %d\n", message.messageType);
    fprintf(stdout, "Number of occurences:%d\n", message.occurrencesNumber);
    fprintf(stdout, "Positions:");
    for(int i=0; i < message.occurrencesNumber; i++)
      fprintf(stdout, " %d\n", message.occurrencesPosition[i]);
    fprintf(stdout, "\n");
    break;
  case END_GAME_TYPE:
    fprintf(stdout, "Message Type: %d\n", message.messageType);
  default:
    fprintf(stdout, "Invalid message type\n");
    break;
  }
}

void receiveMessage(int streamSocket, Message *message){

  if(recv(streamSocket, message, sizeof(message), 0) != sizeof(message))
      dieWithMessage(__FILE__, __LINE__, "error: recv(): %s",strerror(errno));

/*   FILE *instream = fdopen(streamSocket, "r");

  memset(message, 0, sizeof(Message));

  //fread(message, sizeof(Message), 1, instream);
  if(fread(message, sizeof(Message), 1, instream) != 1)
    dieWithMessage(__FILE__, __LINE__, "error: fread(): %s",strerror(errno));
*/
}

void sendMessage(int streamSocket, Message *message){
  if(send(streamSocket, message, sizeof(Message), 0) != sizeof(Message))
    dieWithMessage(__FILE__, __LINE__, "error: send(): %s",strerror(errno));
}
