#pragma once
#ifndef LINE_H
#define LINE_H

#include <string>
#include "utils.h"
#include "token.h"

enum LineType {
    LINE_START_MAIN,
    LINE_END_MAIN,
    LINE_EOF,
    LINE_IDENTIFIER,
    LINE_NUMBER,
    LINE_UNKNOWN,
    LINE_VARIABLE_DECLARATION,
    LINE_LOGICAL_OPERATOR,
    LINE_ARITHMETIC_OPERATOR,
    LINE_INPUT_OPERATOR,
    LINE_OUTPUT_OPERATOR,
    LINE_IF,
    LINE_IF_START,
    LINE_IF_END,
    LINE_ELSE,
    LINE_ELSEIF,
};

class Line {
public:
    Line(LineType type, const std::string& value) : type(type), value(value) {}
    LineType getType() const { return type; }
    const std::string& getValue() const { return value; }
    friend std::ostream& operator<<(std::ostream& os, const Line& Line);
    void tokenize(DoubleLinkedList<Token*>& tokens);
private:
    LineType type;
    std::string value;
};

#endif // LINE_H