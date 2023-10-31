#include "token.h"
#include <iostream>
#include <string>

std::string enumToString(TokenType type) {
    switch (type) {
        case TOKEN_START_MAIN: return "TOKEN_START_MAIN";
        case TOKEN_END_MAIN: return "TOKEN_END_MAIN";
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_VARIABLE_DECLARATION: return "TOKEN_VARIABLE_DECLARATION";
        case TOKEN_INPUT_OPERATOR: return "TOKEN_INPUT_OPERATOR";
        case TOKEN_OUTPUT_OPERATOR: return "TOKEN_OUTPUT_OPERATOR";
        case TOKEN_ARITHMETIC_OPERATOR: return "TOKEN_ARITHMETIC_OPERATOR";
        case TOKEN_UNKNOWN: return "TOKEN_UNKNOWN";
        default: return "Unknown TokenType";
    }
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "Token(type=" << enumToString(token.getType()) << ", value=\"" << token.getValue() << "\")";

    return os;
}
