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
///////////////////////////////////////////////////////////////////////////////////////////////////////
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

    printf("RRQ sent : %s \n", filename);


    //Q4.b

    struct sockaddr adresse;
    int block_number = 1;
    socklen_t size_adress = sizeof(adresse);

    char buffer_received[BUFFER_SIZE] = {0};
    char buffer_ack[BUFFER_SIZE] = {0};
    //Q4.c

    int bytes_received;

    while (bytes_received == 516) {

        bytes_received = recvfrom(socket_descriptor, buffer, msg_length, 0 , &adresse, &size_adress);

        if (bytes_received == -1) {
           printf("Error of receiving data \n");
        }

        if (buffer_received[0] == 0 && buffer_received[1] == 3) {
            buffer_ack[0] =  0;
            buffer_ack[1] = 4;
            buffer_ack[3] = block_number;
            block_number++;

        }

    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    char bufferWRQ[BUFFER_SIZE] = {0};
    bufferWRQ[1] = 2;
    sprintf(bufferWRQ + 2, "%s", filename);
    sprintf(bufferWRQ + 3 + strlen(filename), "NETASCII");

    int msg_lengthWRQ = 12 + strlen(filename);
    int socket_descriptorWRQ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    int number_of_characters_sent = sendto(socket_descriptorWRQ, bufferWRQ, msg_lengthWRQ, 0, res->ai_addr, res->ai_addrlen);
    if (number_of_characters_sent == -1) {
        perror("Error sending WRQ");
        return EXIT_FAILURE;
    }

    printf("WRQ sent: %s \n", filename);

    struct sockaddr adresseWRQ;
    socklen_t size_adresseWRQ = sizeof(adresseWRQ);
    char buffer_receivedWRQ[BUFFER_SIZE] = {0};
    int block_numberWRQ = 1;


    char data_packet[BUFFER_SIZE] = {0};
    size_t bytes_read;

    while (bytes_read == 512) {
        int bytes_sent = sendto(socket_descriptorWRQ, data_packet, bytes_read + 4, 0, &adresseWRQ, size_adresseWRQ);
        if (bytes_sent == -1) {
            printf("Error sending data packet\n");
            return EXIT_FAILURE;
        }

        int ack_received = recvfrom(socket_descriptorWRQ, buffer_receivedWRQ, BUFFER_SIZE, 0, &adresseWRQ, &size_adresseWRQ);
        if (ack_received == -1) {
            printf("Error receiving ACK\n");
            return EXIT_FAILURE;
        }

        if (buffer_receivedWRQ[0] == 0 && buffer_receivedWRQ[1] == 4) {
            int block_number_received = (buffer_receivedWRQ[3] << 8) + buffer_receivedWRQ[4];
            if (block_number_received == block_numberWRQ) {
                block_numberWRQ++;
            }
        }
    }


 return 0;
}
