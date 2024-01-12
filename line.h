#pragma once
#ifndef LINE_H
#define LINE_H

#include <string>
#include <vector>
#include "utils.h"
#include "token.h"

enum LineType {
    LINE_START_MAIN, // done
    LINE_END_MAIN, // done
    LINE_EOF, // --
    LINE_IDENTIFIER, // --
    LINE_NUMBER, // --
    LINE_UNKNOWN, // --
    LINE_VARIABLE_DECLARATION, // done
    LINE_LOGICAL_OPERATOR, // done
    LINE_ARITHMETIC_OPERATOR, // done
    LINE_INPUT_OPERATOR, // done
    LINE_OUTPUT_OPERATOR, // done
    LINE_IF, // done
    LINE_IF_START, // done
    LINE_IF_END, // done
    LINE_ELSE,// done
    LINE_ELSEIF, // done
    LINE_FOR,
    LINE_FOR_BEGIN,
    LINE_FOR_END,
    LINE_WHILE,
    LINE_WHILE_BEGIN,
    LINE_WHILE_END,
};

class Line {
public:
    Line(LineType type, const std::string& value) : type(type), value(value) {}
    LineType getType() const { return type; }
    const std::string& getValue() const { return value; }
    friend std::ostream& operator<<(std::ostream& os, const Line& Line);
    void tokenize(DoubleLinkedList<Token*>& tokens);
    void addData(std::string data) { this->data.push_back(data); }
    std::vector<std::string> getData() { return data; }
private:
    LineType type;
    std::string value;
    std::vector<std::string> data;
};

#endif // LINE_H