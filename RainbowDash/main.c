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

#define port	1100

char** getServerIP () {
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
            printf("%s IP4 Adresse %s\n", ifa->ifa_name, addressBuffer);
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            for (int i = 0; i<strlen(addressBuffer); i++) {
                addrToRet[ia][i] = addressBuffer[i];
            }
            ia++;
            printf("%s IP6 Adresse %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return addrToRet;
}

int startServer (char* ip) {
    /****************** SERVER CODE ****************/
    int welcomeSocket, newSocket;
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
    printf("Server auf Adresse %s starten\n", ip);
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket,5)==0)
        printf("Hören...\n");
    else
        printf("Fehler\n");
    
    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
    /*---- Send message to the socket of the incoming connection ----*/
    printf ("Nachricht:\n");
    while (1) {
        for (int i = 0; i<1024; i++) {
            buffer[i] = 0;
        }
        scanf("%s", buffer);
        send(newSocket, buffer, strlen(buffer), 0);
    }
    return 0;
}

int main () {
    printf ("Das Ministerium für Informationsentwicklung des Dritten Reiches\n©1941 Berlin\n\nDas Ponywurm\n\n");
    char** addrToRet = getServerIP();
    printf ("Serveradresse: %s\n", addrToRet[4]);
    startServer(addrToRet[4]);
    return 0;
}
