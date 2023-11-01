#include "token.h"
#include <iostream>
#include <string>

Token::Token(TokenType type, const std::string& value) : type(type), value(value) {}

TokenType Token::getType() const {
    return type;
}

std::string enumToString(TokenType type) {
    switch (type)
    {
        case TOKEN_CONSTANT: return "CONSTANT";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_VARIABLE: return "VARIABLE";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_LOGICAL_OPERATOR: return "LOGICAL_OPERATOR";
        case TOKEN_VARIABLE_TYPE: return "VARIABLE_TYPE";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "Unknown TokenType";
    }
}

const std::string& Token::getValue() const {
    return value;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "Token(" << enumToString(token.type) << ", " << token.value << ")";
    return os;
}