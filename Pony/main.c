//
//  main.c
//  Pony
//
//  Created by Alexander Nekhaev on 07.03.17.
//  Copyright © 2017 Alexander Nekhaev. All rights reserved.
//

/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void Pearson16(char *x, size_t len, unsigned char res[8], unsigned char T[256]) {
    size_t i;
    size_t j;
    unsigned char h;
    unsigned char hh[8];
    for (j = 0; j < 8; ++j) {
        h = T[(x[0] + j) % 256];
        for (i = 1; i < len; ++i) {
            h = T[h ^ x[i]];
        }
        hh[j] = h;
    }
    for (int i = 0; i<8; i++) {
        res[i] = hh[i];
    }
}

int main(){
    char id[] = "bot1";
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    char w = 1;
    socklen_t addr_size;
    
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(7891);
    /* Set IP address to localhost */
    /*127.0.0.1*/
    serverAddr.sin_addr.s_addr = inet_addr("10.55.122.10");
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    /*---- Establishing connection ----*/
    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    /*---- Send meta data -----*/
    send(clientSocket, id, strlen(id), 0);
    /*---- Read the message from the server into the buffer ----*/
    while (w == 1) {
        recv(clientSocket, buffer, 1024, 0);
        printf ("%s\n", buffer);
    }
    return 0;
}
