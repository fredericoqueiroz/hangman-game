#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "TCPUtility.h"

void dieWithMessage(const char * file_name,int line_number, const char * format, ...){
    va_list vargs;
    va_start(vargs, format);
    fprintf(stderr, "%s:%d: ",file_name, line_number);
    vfprintf(stderr, format, vargs);
    fprintf (stderr, ".\n");
    va_end (vargs);
    exit (1);
}

void handleServerGame(int clientSocket, char * word){
    int buffer[BUFSIZE];
    memset(&buffer, -1, sizeof(buffer));

    //Sending the begin game message (type 1)
    buffer[0] = 1;
    buffer[1] = strlen(word);
    //ssize_t numBytesSend = send(clientSocket, buffer, sizeof(buffer), 0);
    send(clientSocket, buffer, sizeof(buffer), 0);

    //Handle the guess and result messages (type 2 and type 3)
    while(1){
        memset(&buffer, -1, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        printf("Guess Recived (Type | Guess): %d | %d\n", buffer[0], buffer[1]);
        //HANDLE GUESS FUNCTION
            // itera sobre a palavra verificando se o palpite existe na posicao i da palavra
                // se sim
                    // seta a posicao i+2 do buffer da mensagem de resposta
                    // incrementa numero de ocorrencias
                // se nao
                    // verifica a posicao seguinte da palavra (nao faz nada)
            
            // envia mensagem de resposta
    


    }
}
