//
//  parser.h
//  FaceOS
//
//  Created by Alex on 29.01.17.
//  Copyright © 2017 JL Computer Inc. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef char* string;

char* inputString();
char* brkFind (char* input, int n);
char* fromWordToEnd (char* input, int n);
int interprete(char* lexem);
unsigned long int commander(char* cmd);

#endif /* parser_h */
