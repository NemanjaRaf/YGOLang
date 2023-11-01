#ifndef LEXER_H
#define LEXER_H

#pragma once
#include <cstdio>
#include <regex>
#include "line.h"
#include "token.h"
#include "utils.h"

class Lexer {
public:
    Lexer(FILE *fp);
    Line* nextLine();
    ~Lexer();
    DoubleLinkedList<Line*> getLines();
    DoubleLinkedList<Token*> getTokens();
    void printLines();
    void printTokens();
private:
    std::string buffer;
    size_t position;
    FILE *fp;
    DoubleLinkedList<Line*> lines;
    DoubleLinkedList<Token*> tokens;
    void readLines();
};


#endif // LEXER_H