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
