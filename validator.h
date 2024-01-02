#ifndef VALIDATOR_H
#define VALIDATOR_H

#pragma once

#include "token.h"
#include "lexer.h"
#include "models/variable.h"
#include <vector>
#include <iostream>
#include <string>

class Validator {
public:
    Validator(Lexer* lexer);
    bool run();
    ~Validator();
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
    bool validateIfs();
    bool validateFors();
    bool validateWhiles();
    bool validateLines();
    Variable getVariable(std::string name);
};

#endif // VALIDATOR_H
