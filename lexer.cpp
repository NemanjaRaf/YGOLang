// lexer.cpp
#include "lexer.h"
#include "utils.cpp"
#include <sstream>
#include <iostream>


Lexer::Lexer(FILE* fp) : position(0) {
    if (!fp) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    std::ostringstream ss;
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        std::string lineStr(line);
        ss << lineStr;
        std::cout << ss.str() << std::endl;
    }

    std::cout << "==============" << std::endl;

    buffer = ss.str();
    readTokens();
}

Lexer::~Lexer() {
    if (fp) {
        fclose(fp);
    }
}

Token* Lexer::nextToken() {
    if (position >= buffer.size()) {
        return new Token(TOKEN_EOF, "");
    }


    std::string::size_type endOfLine = buffer.find('\n', position);
    if (endOfLine == std::string::npos) {
        endOfLine = buffer.size();
    }

    std::string line = buffer.substr(position, endOfLine - position);
    std::string::const_iterator start = line.begin();
    std::string::const_iterator end = line.end();

    // if empty line, skip
    if (std::regex_search(start, end, std::regex(R"(^\s*$)"))) {
        position = endOfLine + 1; // Move to the next line
        return nextToken();
    }

    std::smatch match;

    if (std::regex_search(start, end, match, std::regex(R"(Let's duel!)"))) {
        position = endOfLine + 1; // Move to the next line
        return new Token(TOKEN_START_MAIN, "Let's duel!");
    }

    // Handle variable declarations
    std::regex variableDeclarationRegex(R"(Card Summon ([\s]*)([\wa-zA-Z\s-]+) As ([\w\s]+) with power ([\w]+)\.)");
    if (std::regex_search(start, end, match, variableDeclarationRegex)) {
        position = endOfLine + 1; // Move to the next line

        std::string variableName = match[2];
        std::string variableType = match[3];
        std::string variableValue = match[4];
        return new Token(TOKEN_VARIABLE_DECLARATION, "Card Summon " + variableName + " As " + variableType + " with power " + variableValue);
    }

    // Handle input operator
    if (std::regex_search(start, end, match, std::regex(R"(([\s]*)([\wa-zA-Z\s-]+), Absorb Power!)"))) {
        position = endOfLine + 1; // Move to the next line
        std::string variableName = match[2];
        return new Token(TOKEN_INPUT_OPERATOR, variableName + ", Absorb Power!");
    }

    // Handle output operator
    if (std::regex_search(start, end, match, std::regex(R"(Draw Phase: Reveal (Spell Card (\"[\w\s]*\")|[\w\s]+)\.)"))) {
        position = endOfLine + 1; // Move to the next line
        std::string output = match[2];
        return new Token(TOKEN_OUTPUT_OPERATOR, "Draw Phase: Reveal " + output);
    }

    // Handle arithmetic operators
    if (std::regex_search(start, end, match, std::regex(R"(([\s]*)([\w\s\-]+)(Attacks|Defends|Copies|Duels|Combines With|Splits By)([\w\s\-]+)!)"))) {
        position = endOfLine + 1; // Move to the next line
        std::string variableName1 = match[2];
        std::string operatorName = match[3];
        std::string variableName2 = match[4];
        return new Token(TOKEN_ARITHMETIC_OPERATOR, variableName1 + " " + operatorName + " " + variableName2 + "!");
    }

    if (std::regex_search(start, end, match, std::regex(R"(I end my turn and the duel!)"))) {
        position = endOfLine + 1; // Move to the next line
        return new Token(TOKEN_END_MAIN, "I end my turn and the duel!");
    }

    position = endOfLine + 1; // Skip to the next line
    return new Token(TOKEN_UNKNOWN, "");
}

void Lexer::readTokens() {
    Token* token;
    do {
        token = nextToken();
        if (token->getType() != TOKEN_EOF)
            tokens.push_back(token);
    } while (token->getType() != TOKEN_EOF);
}

DoubleLinkedList<Token*> Lexer::getTokens() {
    return tokens;
}

void Lexer::printLines() {
    getTokens().print();
}