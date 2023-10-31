#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    TOKEN_START_MAIN,
    TOKEN_END_MAIN,
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_UNKNOWN,
    TOKEN_VARIABLE_DECLARATION,
    TOKEN_ARITHMETIC_OPERATOR,
    TOKEN_INPUT_OPERATOR,
    TOKEN_OUTPUT_OPERATOR,
};

class Token {
public:
    Token(TokenType type, const std::string& value) : type(type), value(value) {}
    TokenType getType() const { return type; }
    const std::string& getValue() const { return value; }
    friend std::ostream& operator<<(std::ostream& os, const Token& token);
private:
    TokenType type;
    std::string value;
};

#endif // TOKEN_H