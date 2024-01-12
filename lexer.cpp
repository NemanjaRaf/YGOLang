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

    // is starts with --, skip
    if (std::regex_search(start, end, std::regex(R"(^--.*)"))) {
        position = endOfLine + 1; 
        return nextLine();
    }

    // if empty line, skip
    if (std::regex_search(start, end, std::regex(R"(^\s*$)"))) {
        position = endOfLine + 1; 
        return nextLine();
    }

    std::smatch match;

    if (std::regex_search(start, end, match, std::regex(R"(Let's duel!)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_START_MAIN, "Let's duel!");
    }

    // Handle variable declarations
    std::regex variableDeclarationRegex(R"(Card Summon ([\s]*)([\wa-zA-Z\s-]+) As ([\w\s]+) with (power ([\w\.\']+)|([\w\s]+) links ([\w\,\.\']+))\.)");
    if (std::regex_search(start, end, match, variableDeclarationRegex)) {
        position = endOfLine + 1; 

        Line l = Line(LINE_VARIABLE_DECLARATION, match[0]);
        l.addData(match[2]);
        l.addData(match[3]);
        l.addData(match[5]);
        l.addData(match[6]);
        l.addData(match[7]);
        return new Line(l);
    }

    // Handle input operator
    if (std::regex_search(start, end, match, std::regex(R"(([\s]*)([\wa-zA-Z\s-]+), Absorb Power!)"))) {
        position = endOfLine + 1; 
        std::string variableName = match[2];
        Line l = Line(LINE_INPUT_OPERATOR, match[0]);
        l.addData(variableName);
        return new Line(l);
    }

    // Handle output operator
    if (std::regex_search(start, end, match, std::regex(R"(Draw Phase: Reveal (Spell Card (\"[\w\s\d]*\")|[\w\s\d-]+)\.)"))) {
        position = endOfLine + 1; 
        std::string output = match[1];
        Line l = Line(LINE_OUTPUT_OPERATOR, match[0]);
        l.addData(output);
        return new Line(l);
    }

    // Handle arithmetic operators
    if (std::regex_search(start, end, match, std::regex(R"(([\s]*)([\w\s\-]+)(Attacks|Defends|Copies|Duels|Combines With|Splits By)([\w\s\-]+)!)"))) {
        position = endOfLine + 1; 
        std::string variableName1 = match[2];
        std::string operatorName = match[3];
        std::string variableName2 = match[4];
        Line l = Line(LINE_ARITHMETIC_OPERATOR, match[0]);
        l.addData(variableName1);
        l.addData(operatorName);
        l.addData(variableName2);
        return new Line(l);
    }

    // Handle if statements
    if (std::regex_search(start, end, match, std::regex(R"(Activate Trap: Mirror Force)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_IF, "Activate Trap: Mirror Force");
    }

    // Handle else statements
    if (std::regex_search(start, end, match, std::regex(R"(Counter Trap: Negate Attack)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_ELSE, "Counter Trap: Negate Attack");
    }

    //handle else if statements
    if (std::regex_search(start, end, match, std::regex(R"(Quick-Play:([\w\s\-\!\,]*))"))) {
        position = endOfLine + 1; 
        std::string logicalOperator = match[1];
        Line l = Line(LINE_ELSEIF, "Quick-Play:" + logicalOperator);
        // regex to get the logical operator
        start = logicalOperator.begin();
        end = logicalOperator.end();
        // mattch is logical operator
        match = std::smatch();

        if (std::regex_search(start, end, match, std::regex(R"(^([\w\s\-]+)\s+and\s+([\w\s\-]+),\s+Engage\s+in\s+Duel\s+for\s+([\w\s\-]+)[!,]?\s*)"))) {
            std::string variableName1 = match[1];
            std::string variableName2 = match[2];
            std::string operation = match[3];
            l.addData(variableName1);
            l.addData(variableName2);
            l.addData(operation);
        }


        return new Line(l);
    }

    if (std::regex_search(start, end, match, std::regex(R"(Start Phase:)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_IF_START, "Start Phase:");
    } 
    if (std::regex_search(start, end, match, std::regex(R"(End Phase.)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_IF_END, "End Phase.");
    }

    if (std::regex_search(start, end, match, std::regex(R"(I end my turn and the duel!)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_END_MAIN, "I end my turn and the duel!");
    }

    // Handle logical operators
    if (std::regex_search(start, end, match, std::regex(R"((?:\s*([\w\s\-]+)\s+and\s+([\w\s\-]+),\s+Engage\s+in\s+Duel\s+for\s+([\w\s\-]+)[!,]?\s*)+)"))) {
        // read line from start to end
        std::string line = buffer.substr(position, endOfLine - position);
        position = endOfLine + 1; 
        std::string variableName1 = match[1];
        std::string variableName2 = match[2];
        std::string operation = match[3];
        Line l = Line(LINE_LOGICAL_OPERATOR, line);
        l.addData(variableName1);
        l.addData(variableName2);
        l.addData(operation);
        return new Line(l);
    }

    // Handle for loops
    if (std::regex_search(start, end, match, std::regex(R"(Activate Continuous Spell: Magic Formula from ([\w\s\-\d]+) To ([\w\s\-\d]+) With ([\w\s\-\d]+) increment\.)"))) {
        position = endOfLine + 1; 
        std::string variableName1 = match[1];
        std::string variableName2 = match[2];
        std::string variableName3 = match[3];
        Line l = Line(LINE_FOR, match[0]);
        l.addData(variableName1);
        l.addData(variableName2);
        l.addData(variableName3);
        return new Line(l);
    }

    // Handle for begin
    if (std::regex_search(start, end, match, std::regex(R"(Tap Continuous Spell: Magic Formula.)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_FOR_BEGIN, match[0]);
    }

    // Handle for end
    if (std::regex_search(start, end, match, std::regex(R"(End Continuous Spell: Magic Formula.)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_FOR_END, match[0]);
    }

    // Handle while loops
    if (std::regex_search(start, end, match, std::regex(R"(Activate Continuous Trap: Imperial Order based on previous duel!)"))) {
        position = endOfLine + 1;
        return new Line(LINE_WHILE, "Activate Continuous Trap: Imperial Order based on previous duel!");
    }

    // Handle while begin
    if (std::regex_search(start, end, match, std::regex(R"(Check Continuous Trap: Imperial Order!)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_WHILE_BEGIN, match[0]);
    }

    // Handle while end
    if (std::regex_search(start, end, match, std::regex(R"(End Continuous Trap: Imperial Order!)"))) {
        position = endOfLine + 1; 
        return new Line(LINE_WHILE_END, match[0]);
    }

    position = endOfLine + 1;
    return new Line(LINE_UNKNOWN, line);
}

void Lexer::readLines() {
    Line* Line;
    do {
        Line = nextLine();
        if (Line->getType() != LINE_EOF)
            Line->tokenize(tokens);
            lines.push_back(Line);
    } while (Line->getType() != LINE_EOF);
    lines.pop_back();
}

DoubleLinkedList<Line*>* Lexer::getLines() {
    return &lines;
}

DoubleLinkedList<Token*> Lexer::getTokens() {
    return tokens;
}

void Lexer::printLines() {
    getLines()->print();
}

void Lexer::printTokens() {
    getTokens().print();
}