#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "line.h"
#include "token.h"
#include "ast.h"
#include <iostream>

class Parser {
    public:
        Parser(Lexer* lexer);
        AST* parse();
        ~Parser();
    private:
        Lexer* lexer;
        AST* ast;
};

#endif // PARSER_H
