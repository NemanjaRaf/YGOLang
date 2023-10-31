#ifndef LEXER_H
#define LEXER_H

#pragma once
#include <cstdio>
#include <regex>
#include "token.h"
#include "utils.h"

class Lexer {
public:
    Lexer(FILE *fp);
    Token* nextToken();
    ~Lexer();
    DoubleLinkedList<Token*> getTokens();
    void printLines();
private:
    std::string buffer;
    size_t position;
    FILE *fp;
    DoubleLinkedList<Token*> tokens;
    void readTokens();
};


#endif // LEXER_H