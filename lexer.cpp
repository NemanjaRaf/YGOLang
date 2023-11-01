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
        // std::cout << ss.str() << std::endl;
    }

    // std::cout << "==============" << std::endl;

    buffer = ss.str();
    readLines();
}

Lexer::~Lexer() {
    if (fp) {
        fclose(fp);
    }
}

Line* Lexer::nextLine() {
    if (position >= buffer.size()) {
        return new Line(LINE_EOF, "");
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
        return nextLine();
    }

    std::smatch match;

    if (std::regex_search(start, end, match, std::regex(R"(Let's duel!)"))) {
        position = endOfLine + 1; // Move to the next line
        return new Line(LINE_START_MAIN, "Let's duel!");
    }

    // Handle variable declarations
    std::regex variableDeclarationRegex(R"(Card Summon ([\s]*)([\wa-zA-Z\s-]+) As ([\w\s]+) with power ([\w]+)\.)");
    if (std::regex_search(start, end, match, variableDeclarationRegex)) {
        position = endOfLine + 1; // Move to the next line

        std::string variableName = match[2];
        std::string variableType = match[3];
        std::string variableValue = match[4];
        return new Line(LINE_VARIABLE_DECLARATION, "Card Summon " + variableName + " As " + variableType + " with power " + variableValue);
    }

    // Handle input operator
    if (std::regex_search(start, end, match, std::regex(R"(([\s]*)([\wa-zA-Z\s-]+), Absorb Power!)"))) {
        position = endOfLine + 1; // Move to the next line
        std::string variableName = match[2];
        return new Line(LINE_INPUT_OPERATOR, variableName + ", Absorb Power!");
    }

    // Handle output operator
    if (std::regex_search(start, end, match, std::regex(R"(Draw Phase: Reveal (Spell Card (\"[\w\s]*\")|[\w\s]+)\.)"))) {
        position = endOfLine + 1; // Move to the next line
        std::string output = match[1];
        return new Line(LINE_OUTPUT_OPERATOR, "Draw Phase: Reveal " + output);
    }

    // Handle arithmetic operators
    if (std::regex_search(start, end, match, std::regex(R"(([\s]*)([\w\s\-]+)(Attacks|Defends|Copies|Duels|Combines With|Splits By)([\w\s\-]+)!)"))) {
        position = endOfLine + 1; // Move to the next line
        std::string variableName1 = match[2];
        std::string operatorName = match[3];
        std::string variableName2 = match[4];
        return new Line(LINE_ARITHMETIC_OPERATOR, variableName1 + "" + operatorName + "" + variableName2 + "!");
    }

    // Handle logical operators
    if (std::regex_search(start, end, match, std::regex(R"((?:\s*([\w\s\-]+)\s+and\s+([\w\s\-]+),\s+Engage\s+in\s+Duel\s+for\s+([\w\s\-]+)[!,]?\s*)+)"))) {
        // read line from start to end
        std::string line = buffer.substr(position, endOfLine - position);
        position = endOfLine + 1; // Move to the next line
        std::string variableName1 = match[1];
        std::string variableName2 = match[2];
        std::string variableName3 = match[3];
        return new Line(LINE_LOGICAL_OPERATOR, line);
    }

    // Handle if statements
    if (std::regex_search(start, end, match, std::regex(R"(Activate Trap: Mirror Force)"))) {
        position = endOfLine + 1; // Move to the next line
        return new Line(LINE_IF, "Activate Trap: Mirror Force");
    }
    if (std::regex_search(start, end, match, std::regex(R"(Start Phase:)"))) {
        position = endOfLine + 1; // Move to the next line
        return new Line(LINE_IF, "Start Phase:");
    } 
    if (std::regex_search(start, end, match, std::regex(R"(End Phase.)"))) {
        position = endOfLine + 1; // Move to the next line
        return new Line(LINE_IF, "End Phase.");
    }

    if (std::regex_search(start, end, match, std::regex(R"(I end my turn and the duel!)"))) {
        position = endOfLine + 1; // Move to the next line
        return new Line(LINE_END_MAIN, "I end my turn and the duel!");
    }

    position = endOfLine + 1; // Skip to the next line
    return new Line(LINE_UNKNOWN, "");
}

void Lexer::readLines() {
    Line* Line;
    do {
        Line = nextLine();
        if (Line->getType() != LINE_EOF)
            Line->tokenize(tokens);
            lines.push_back(Line);
    } while (Line->getType() != LINE_EOF);
}

DoubleLinkedList<Line*> Lexer::getLines() {
    return lines;
}

DoubleLinkedList<Token*> Lexer::getTokens() {
    return tokens;
}

void Lexer::printLines() {
    getLines().print();
}

void Lexer::printTokens() {
    getTokens().print();
}