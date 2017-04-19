//
//  main.c
//  RainbowDash
//
//  Created by Alex on 02.03.17.
//  Copyright © 2017 JL Computer Inc. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "parser.h"
#include <curses.h>
#include <menu.h>

#define port	1100
#define MAXBUF 65536

int client = 0;
unsigned char hash[256];

char* buffer = NULL;
bool flag = false;
bool ui = false;
char* var = NULL;
char* constant = NULL;

void broadcast (char* message) {
    int sock, status, buflen, sinlen;
    char buffer[MAXBUF];
    struct sockaddr_in sock_in;
    int yes = 1;
    
    int port1 = 7892;
    
    sinlen = sizeof(struct sockaddr_in);
    memset(&sock_in, 0, sinlen);
    buflen = MAXBUF;
    
    sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_in.sin_port = htons(7892);
    sock_in.sin_family = PF_INET;
    
    status = bind(sock, (struct sockaddr *)&sock_in, sinlen);
    printf("Bind Status = %d\n", status);
    
    status = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
    printf("Setsockopt Status = %d\n", status);
    
    /* -1 = 255.255.255.255 this is a BROADCAST address,
     a local broadcast address could also be used.
     you can comput the local broadcat using NIC address and its NETMASK
     */
    
    sock_in.sin_addr.s_addr=htonl(-1); /* send message to 255.255.255.255 */
    sock_in.sin_port = htons(port1); /* port number */
    sock_in.sin_family = PF_INET;
    
    sprintf(buffer, "%s", message);
    buflen = (int)strlen(buffer);
    status = (int)sendto(sock, buffer, buflen, 0, (struct sockaddr *)&sock_in, sinlen);
    printf("sendto Status = %d\n", status);
    shutdown(sock, 2);
    close(sock);
}

char* getServerIP () {
    //printf ("Server information:\n");
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    char** addrToRet;
    addrToRet = malloc(sizeof(char*) * 6);
    for (int j = 0; j<6; j++) {
        addrToRet[j] = malloc(sizeof(char)*INET6_ADDRSTRLEN);
    }
    getifaddrs(&ifAddrStruct);
    int ia = 0;
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            for (int i = 0; i<strlen(addressBuffer); i++) {
                addrToRet[ia][i] = addressBuffer[i];
            }
            ia++;
            //printf("%s IP4 Adress %s\n", ifa->ifa_name, addressBuffer);
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            for (int i = 0; i<strlen(addressBuffer); i++) {
                addrToRet[ia][i] = addressBuffer[i];
            }
            ia++;
            //printf("%s IP6 Adress %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return addrToRet[4];
}

int startServer (char* ip) {
    /****************** SERVER CODE ****************/
    int welcomeSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(7891);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    /*---- Bind the address struct to the socket ----*/
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    printf("Server on %s started\n", ip);
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket,5)==0)
        printf("Listening...\n");
    else
        printf("Error\n");
    
    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof serverStorage;
    client = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
    recv(client, buffer, 1024, 0);
    printf("Connection with %s established\n", buffer);
    /*---- Send message to the socket of the incoming connection ----*/
    printf ("Ready for transmission\n");
    return client;
}

void processor(unsigned long int code) {
    unsigned long int workingCopy = code;
    //transcode to string
    char line[20];
    int i = 0;
    for (int i = 0; i<20; i++) {
        line[i] = 0;
    }
    while (workingCopy != 0) {
        line[i] = '0'+workingCopy % 10;
        workingCopy = workingCopy/10;
        i++;
    }
    for (int i = 0; i<20; i++) {
        if (line[i] == '1') {
            printf("%s\n", buffer);
        }
        if (line[i] == '4') {
            recv(client, buffer, 1024, 0);
        }
        if (line[i] == '9') {
            flag = true;
        }
        if (line[i] == '8') {
            buffer = inputString();
        }
        if (line [i] == '7') {
            buffer = getServerIP();
        }
        if (line[i] == '2') {
            startServer(buffer);
        }
        if (line[i] == '6') {
            var = buffer;
        }
        if (line[i] == '3') {
            send(client, line, strlen(line)-1, 0);
            break;
        }
        if (line[i] == '0') {
            if (line[i+1] == '0') {
                flag = false;
            }
            if (line[i+1] == '1') {
                ui = flag;
            }
            if (line[i+1] == '2') {
                buffer = var;
            }
            if (line[i+1] == '3') {
                broadcast(buffer);
            }
            if (line[i+1] == '4') {
                buffer = line;
            }
            if (line[i+1] == '5') {
                send(client, buffer, strlen(buffer), 0);
            }
            if (line[i+1] == '6') {
                constant = malloc(sizeof(char)*strlen(buffer));
                for (int i = 0; i<strlen(buffer); i++) {
                    constant[i] = buffer[i];
                }
            }
            if (line[i+1] == '7') {
                buffer = constant;
            }
            if (line[i+1] == '8') {
                free(constant);
            }
            if (line[i+1] == '9') {
                buffer = "RainbowDash Commander version 2\n Command reference:\nprint,\nstart,\nsendc,\nrequest,\nfile,\nsave,\ninfo,\nmsg,\non,\noff,\nexec,\nvar,\nbroadcast,\nline,\nsends,\nconstant,\nreadconst,\nfreemem\n";
            }
            i++;
        }
    }
}

int main () {
    printf ("©2017 MIPT\n");
    unsigned long int toSend = 0;
    while (1==1) {
        string cmd = inputString();
        toSend = commander(cmd);
        processor(toSend);
    }
    return 0;
}
