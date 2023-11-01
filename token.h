#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

enum TokenType {
    TOKEN_CONSTANT,
    TOKEN_KEYWORD,
    TOKEN_VARIABLE,
    TOKEN_OPERATOR,
    TOKEN_LOGICAL_OPERATOR,
    TOKEN_VARIABLE_TYPE,
    TOKEN_UNKNOWN,
};

class Token {
public:
    Token(TokenType type, const std::string& value);
    TokenType getType() const;
    const std::string& getValue() const;
    friend std::ostream& operator<<(std::ostream& os, const Token& token);

private:
    TokenType type;
    std::string value;
};

#endif
