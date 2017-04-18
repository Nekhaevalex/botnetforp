//
//  parser.c
//  FaceOS
//
//  Created by Alex on 29.01.17.
//  Copyright © 2017 JL Computer Inc. All rights reserved.
//

#include "parser.h"

typedef enum commands {
    nil,
    self,
    start,
    sendc,
    request,
    file,
    save,
    info,
    msg,
    on,
    off,
    ui,
} commands;

char* inputString() {
    char buff[1024];
    for (int i = 0; i<1024; i++)
        buff[i] = 0;
    char* string;
    unsigned long int length = 1;
    fgets(buff, 1024, stdin);
    length = strlen(buff);
    string = malloc((length)*sizeof(char));
    for (int i = 0; i<=length; i++) {
        string[i] = buff[i];
    }
    if (string[strlen(string)-1] == '\n') {
        string[strlen(string)-1] = 0;
    }
    return string;
}

char* wordFinder (char* input, int n) {
    char* toSend;
    long int length = strlen(input);
    int beginCut = 0;
    int endCut = 0;
    int brksFound = 0;
    for (int i = 0; i<length; i++) {
        if (input[i] == ' ')
            brksFound++;
    }
    int* brkLinks = malloc(sizeof(int)*(brksFound+2));
    brkLinks[0] = 0;
    int j = 1;
    for (int i = 0; i<length; i++) {
        if (input[i] == ' ') {
            brkLinks[j] = i+1;
            j++;
        }
    }
    brkLinks[brksFound+1] = (int)length+2;
    beginCut = brkLinks[n-1];
    endCut = brkLinks[n]-2;
    toSend = malloc(sizeof(char)*(endCut-beginCut));
    for (int i = beginCut; i<=endCut; i++) {
        toSend[i-beginCut] = input[i];
    }
    free(brkLinks);
    return toSend;
}

char* fromWordToEnd (char* input, int n) {
    char* toSend;
    long int length = strlen(input);
    int beginCut = 0;
    long int endCut = length;
    int brksFound = 0;
    for (int i = 0; i<length; i++) {
        if (input[i] == ' ') {
            brksFound++;
            if (brksFound == n) {
                beginCut = i+1;
                break;
            }
        }
    }
    if (beginCut == endCut || brksFound == 0) {
        return NULL;
    }
    else {
        toSend = malloc(sizeof(char)*(endCut-beginCut+1));
        for (int i = beginCut; i<endCut; i++) {
            toSend[i-beginCut] = input[i];
        }
        toSend[endCut+1] = '\0';
        return toSend;
    }
}
int interprete(char* lexem) {
    if (strcmp(lexem, "sendc")==0){
        return sendc;
    }
    else if (strcmp(lexem, "request")==0) {
        return request;
    }
    else if (strcmp(lexem, "file")==0) {
        return file;
    }
    else if (strcmp(lexem, "info")==0) {
        return info;
    }
    else if (strcmp(lexem, "msg")==0) {
        return msg;
    }
    else if (strcmp(lexem, "self")==0) {
        return self;
    }
    else if (strcmp(lexem, "start")==0) {
        return start;
    }
    else if (strcmp(lexem, "on")==0) {
        return on;
    }
    else if (strcmp(lexem, "off")==0) {
        return off;
    }
    else if (strcmp(lexem, "ui")==0) {
        return ui;
    }
    else if (strcmp(lexem, "save")==0) {
        return save;
    }
    else {
        return nil;
    }
}
unsigned long int commander(char* cmd) {
    int len = (int)strlen(cmd);
    int words = 0;
    for (int i = 0; i<len; i++) {
        if (cmd[i] == ' ') {
            words++;
        }
    }
    words++;
    unsigned long int c = 0;
    unsigned long int set = 0;//init cmd
    for (int i = 0; i<words; i++) {
        string cache = wordFinder(cmd, i+1);
        c = interprete(cache);
        if (c>9) {
            set=set*1000+c*1000;
        }
        else {
            set=set*10+c;
        }
    }
    return set;
}
