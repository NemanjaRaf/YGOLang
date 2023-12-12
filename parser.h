#ifndef PARSER_H
#define PARSER_H

#pragma once

#include "token.h"
#include "lexer.h"
#include "models/variable.h"
#include <vector>
#include <iostream>
#include <string>

class Parser {
public:
    Parser(Lexer* lexer);
    void parse();
    ~Parser();
    void addVariable(Variable variable) { variables.push_back(variable); }
    std::vector<Variable> getVariables() { return variables; }
    bool isVariableDeclared(std::string name);
private:
    Lexer* lexer;
    std::vector<Variable> variables;
    void parseVariables();
    bool validate();

    bool validateMain();
    bool validateVariables();
    bool validateInput();
    bool validateOutput();
};

#endif // PARSER_H
