#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "main.h"


int main(int argc, char **argv) {
    //Q1
    char *host = argv[1];
    char *port = argv[2];
    char *filename = argv[3];

    if (argc != 4) {
        fprintf(stderr, "there are %d arguments instead of 3 \n", argc);
        return EXIT_FAILURE;
    }
    //printf("Send %s to host : %s@%s \n", filename, host, port);

    //Q2
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family=AF_INET;
    hints.ai_protocol=IPPROTO_UDP;

    int status = getaddrinfo(host, port, &hints, &res);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }

    //QA.a

    char buffer[BUFFER_SIZE] = {0};
    buffer[1]=1;
    sprintf(buffer+2, "%s", filename);
    sprintf( buffer + 3 + strlen(filename), "NETASCII");

    int socket_descriptor = socket(res->ai_family, res->ai_socktype,res->ai_protocol);
    int msg_length = 12+strlen(filename);

    int number_of_character_sent = sendto(socket_descriptor, buffer, msg_length, 0, res->ai_addr, res->ai_addrlen);
    if (number_of_character_sent == -1){
        printf("Error of character number \n");
        return EXIT_FAILURE;
    }
    printf("Message sent\n");


    //Q4.b

    struct sockaddr adresse;
    int block_number = 1;
    socklen_t size_adress = sizeof(adresse);

    //int bytes_received = recvfrom(socket_descriptor, buffer, msg_length, NULL, &adresse, &size_adress);

    //if (bytes_received == -1) {
    //    printf("Error of receiving data \n");
    //}

    char buffer_received[BUFFER_SIZE] = {0};
    //buffer_received[1] = 3;
    //sprintf(buffer_received + 2, "%s", block_number);
    //sprintf(buffer_received + 3 + strlen(block_number), strlen(bytes_received));

    char buffer_ack[BUFFER_SIZE] = {0};
    //buffer_ack[1] = 4;
    //sprintf(buffer_ack + 2 , "%s", block_number);

    //Q4.c

    int bytes_received;

    do {
        bytes_received = recvfrom(socket_descriptor, buffer, msg_length, NULL, &adresse, &size_adress);
        if (bytes_received == -1) {
            printf("Error of receiving data \n");
        }

        if (buffer_received[0] == 0 && buffer_received[1] == 3) {
            buffer_ack[0] =  0;
            buffer_ack[1] = 4;
            buffer_ack[3] = block_number;
            block_number++;
        }

    }while (bytes_received == 512);


}
