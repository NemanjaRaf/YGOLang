#include "line.h"
#include "utils.cpp"
#include "token.h"
#include <regex>
#include <iostream>
#include <string>

std::string enumToString(LineType type) {
    switch (type) {
        case LINE_START_MAIN: return "LINE_START_MAIN";
        case LINE_END_MAIN: return "LINE_END_MAIN";
        case LINE_EOF: return "LINE_EOF";
        case LINE_IDENTIFIER: return "LINE_IDENTIFIER";
        case LINE_VARIABLE_DECLARATION: return "LINE_VARIABLE_DECLARATION";
        case LINE_INPUT_OPERATOR: return "LINE_INPUT_OPERATOR";
        case LINE_OUTPUT_OPERATOR: return "LINE_OUTPUT_OPERATOR";
        case LINE_ARITHMETIC_OPERATOR: return "LINE_ARITHMETIC_OPERATOR";
        case LINE_LOGICAL_OPERATOR: return "LINE_LOGICAL_OPERATOR";
        case LINE_IF: return "LINE_IF";
        case LINE_IF_START: return "LINE_IF_START";
        case LINE_IF_END: return "LINE_IF_END";
        case LINE_ELSE: return "LINE_ELSE";
        case LINE_ELSEIF: return "LINE_ELSEIF";
        case LINE_FOR: return "LINE_FOR";
        case LINE_FOR_BEGIN: return "LINE_FOR_BEGIN";
        case LINE_FOR_END: return "LINE_FOR_END";
        case LINE_UNKNOWN: return "LINE_UNKNOWN";
        default: return "Unknown LineType";
    }
}

std::ostream& operator<<(std::ostream& os, const Line& Line) {
    os << "Line(type=" << enumToString(Line.getType()) << ", value=\"" << Line.getValue() << "\")";

    return os;
}

static void tokenizeVariable(DoubleLinkedList<Token*>& tokens, std::string variable) {
    // std::cout << "tokenizeVariable: " << variable << std::endl;
    if (std::regex_search(variable, std::regex(R"(Token with power ([\w\s\-]+))"))) {
        std::smatch match;
        std::regex_search(variable, match, std::regex(R"(Token with power ([\w\s\-]+))"));
        tokens.push_back(new Token(TOKEN_KEYWORD, "Token with power"));
        tokens.push_back(new Token(TOKEN_CONSTANT, trim(match[1])));
    }
    else if (std::regex_search(variable, std::regex(R"(Link ([\w\s\-]+) of ([\w\s\-]+))"))) {
        std::smatch match;
        std::regex_search(variable, match, std::regex(R"(Link ([\w\s\-]+) of ([\w\s\-]+))"));
        tokens.push_back(new Token(TOKEN_KEYWORD, "Link"));
        tokens.push_back(new Token(TOKEN_CONSTANT, trim(match[1])));
        tokens.push_back(new Token(TOKEN_KEYWORD, "of"));
        tokens.push_back(new Token(TOKEN_VARIABLE, trim(match[2])));
    }
    else if (std::regex_search(variable, std::regex(R"(Links of ([\w\s\-]+))"))) {
        std::smatch match;
        std::regex_search(variable, match, std::regex(R"(Links of ([\w\s\-]+))"));
        tokens.push_back(new Token(TOKEN_KEYWORD, "Links of"));
        tokens.push_back(new Token(TOKEN_VARIABLE, trim(match[1])));
    } else {
        tokens.push_back(new Token(TOKEN_VARIABLE, trim(variable)));
    }
}

void Line::tokenize(DoubleLinkedList<Token*>& tokens) {
    std::smatch match;
    std::string v = this->getValue();
    switch (this->getType()) {
        case LINE_START_MAIN:
            tokens.push_back(new Token(TOKEN_KEYWORD, "Let's duel!"));
            break;
        case LINE_END_MAIN:
            tokens.push_back(new Token(TOKEN_KEYWORD, "I end my turn and the duel!"));
            break;
        case LINE_VARIABLE_DECLARATION:
            if (std::regex_search(this->getValue(), match, std::regex(R"(Card Summon ([\s]*)([\wa-zA-Z\s-]+) As ([\w\s]+) with (power ([\w]+)|([\w\s]+) links ([\w\,]+))\.)"))) {
                if (match[6] == "") {
                    tokens.push_back(new Token(TOKEN_KEYWORD, "Card Summon"));
                    tokens.push_back(new Token(TOKEN_VARIABLE, this->getValue().substr(12, this->getValue().find(" As ") - 12)));
                    tokens.push_back(new Token(TOKEN_KEYWORD, "As"));
                    tokens.push_back(new Token(TOKEN_VARIABLE_TYPE, this->getValue().substr(this->getValue().find(" As ") + 4, this->getValue().find(" with power ") - this->getValue().find(" As ") - 4)));
                    tokens.push_back(new Token(TOKEN_KEYWORD, "with power"));
                    tokens.push_back(new Token(TOKEN_CONSTANT, this->getValue().substr(this->getValue().find(" with power ") + 12, this->getValue().length() - this->getValue().find(" with power ") - 13)));
                } else {
                    tokens.push_back(new Token(TOKEN_KEYWORD, "Card Summon"));
                    tokens.push_back(new Token(TOKEN_VARIABLE, match[2]));
                    tokens.push_back(new Token(TOKEN_KEYWORD, "As"));
                    tokens.push_back(new Token(TOKEN_VARIABLE_TYPE, match[3]));
                    tokens.push_back(new Token(TOKEN_KEYWORD, "with"));
                    tokens.push_back(new Token(TOKEN_VARIABLE_TYPE, match[6]));
                    tokens.push_back(new Token(TOKEN_KEYWORD, "links"));

                    std::vector<std::string> constants = split(match[7], ',');
                    for (std::string constant : constants) {
                        tokens.push_back(new Token(TOKEN_CONSTANT, constant));
                    }
                }
            }
            
            break;
        case LINE_INPUT_OPERATOR:
            tokens.push_back(new Token(TOKEN_VARIABLE, this->getValue().substr(0, this->getValue().find(", Absorb Power!"))));
            tokens.push_back(new Token(TOKEN_KEYWORD, "Absorb Power!"));
            break;
        case LINE_OUTPUT_OPERATOR:
            tokens.push_back(new Token(TOKEN_KEYWORD, "Draw Phase: Reveal"));
            if (this->getValue().find("Spell Card") != std::string::npos) {
                tokens.push_back(new Token(TOKEN_KEYWORD, "Spell Card"));
                tokens.push_back(new Token(TOKEN_CONSTANT, this->getValue().substr(this->getValue().find("\"") + 1, this->getValue().length() - this->getValue().find("\"") - 2)));
            } else {
                // tokens.push_back(new Token(TOKEN_VARIABLE, this->getValue().substr(this->getValue().find("Reveal ") + 7, this->getValue().length() - this->getValue().find("Reveal ") - 8)));
                tokenizeVariable(tokens, this->getValue().substr(this->getValue().find("Reveal ") + 7, this->getValue().length() - this->getValue().find("Reveal ") - 8));
            }
            break;
        case LINE_ARITHMETIC_OPERATOR:
            if (std::regex_search(this->getValue(), match, std::regex(R"(([\s]*)([\w\s\-]+)(Attacks|Defends|Copies|Duels|Combines With|Splits By)([\s]*)([\w\s\-]+)!)"))) {
                tokens.push_back(new Token(TOKEN_VARIABLE, trim(match[2])));
                tokens.push_back(new Token(TOKEN_KEYWORD, trim(match[3])));
                tokenizeVariable(tokens, trim(match[5]));
                // tokens.push_back(new Token(TOKEN_VARIABLE, trim(match[5])));
            }
            break;
        case LINE_LOGICAL_OPERATOR:
            while (std::regex_search(v, match, std::regex(R"((?:\s*([\w\s\-]+)\s+and\s+([\w\s\-]+),\s+Engage\s+in\s+Duel\s+for\s+([\w\s\-]+)[!,]?\s*)+)"))) {
                // tokens.push_back(new Token(TOKEN_VARIABLE, trim(match[1])));
                tokenizeVariable(tokens, trim(match[1]));
                tokens.push_back(new Token(TOKEN_KEYWORD, "and"));
                // tokens.push_back(new Token(TOKEN_VARIABLE, trim(match[2])));
                tokenizeVariable(tokens, trim(match[2]));
                tokens.push_back(new Token(TOKEN_KEYWORD, "Engage in Duel for"));
                tokens.push_back(new Token(TOKEN_LOGICAL_OPERATOR, trim(match[3])));
                v = match.suffix().str();

                if (std::regex_search(v, match, std::regex(R"((?:\s*(Or|And)\s*)+)"))) {
                    tokens.push_back(new Token(TOKEN_LOGICAL_OPERATOR, trim(match[1])));
                    v = match.suffix().str();
                }

            }
            break;
        case LINE_IF: 
            if (this->getValue() == "Activate Trap: Mirror Force") {
                tokens.push_back(new Token(TOKEN_KEYWORD, "Activate Trap: Mirror Force"));
            }
            break;
        case LINE_IF_START:
            if (this->getValue() == "Start Phase:") {
                tokens.push_back(new Token(TOKEN_KEYWORD, "Start Phase:"));
            }
            break;
        case LINE_IF_END:
            if (this->getValue() == "End Phase.") {
                tokens.push_back(new Token(TOKEN_KEYWORD, "End Phase."));
            }
            break;
        case LINE_ELSE:
            if (this->getValue() == "Counter Trap: Negate Attack") {
                tokens.push_back(new Token(TOKEN_KEYWORD, "Counter Trap: Negate Attack"));
            }
            break;
        case LINE_ELSEIF:
            if (std::regex_search(this->getValue(), match, std::regex(R"(Quick-Play:([\w\s\-\!\,]*))"))) {
                tokens.push_back(new Token(TOKEN_KEYWORD, "Quick-Play:"));

                v = trim(match[1].str());
                while (std::regex_search(v, match, std::regex(R"((?:\s*([\w\s\-]+)\s+and\s+([\w\s\-]+),\s+Engage\s+in\s+Duel\s+for\s+([\w\s\-]+)[!,]?\s*)+)"))) {
                    // tokens.push_back(new Token(TOKEN_VARIABLE, trim(match[1])));
                    tokenizeVariable(tokens, trim(match[1]));
                    tokens.push_back(new Token(TOKEN_KEYWORD, "and"));
                    // tokens.push_back(new Token(TOKEN_VARIABLE, trim(match[2])));
                    tokenizeVariable(tokens, trim(match[2]));
                    tokens.push_back(new Token(TOKEN_KEYWORD, "Engage in Duel for"));
                    tokens.push_back(new Token(TOKEN_LOGICAL_OPERATOR, trim(match[3])));
                    v = match.suffix().str();

                    if (std::regex_search(v, match, std::regex(R"((?:\s*(Or|And)\s*)+)"))) {
                        tokens.push_back(new Token(TOKEN_LOGICAL_OPERATOR, trim(match[1])));
                        v = match.suffix().str();
                    }

                }
            }
            break;
        case LINE_FOR:
            if (std::regex_search(this->getValue(), match, std::regex(R"(Activate Continuous Spell: Magic Formula from ([\w\s\-\d]+) To ([\w\s\-\d]+) With ([\w\s\-\d]+) increment\.)"))) {
                tokens.push_back(new Token(TOKEN_KEYWORD, "Activate Continuous Spell: Magic Formula from"));
                tokenizeVariable(tokens, trim(match[1]));
                std::cout << "match[1]: " << match[1] << std::endl;
                tokens.push_back(new Token(TOKEN_KEYWORD, "To"));
                tokenizeVariable(tokens, trim(match[2]));
                tokens.push_back(new Token(TOKEN_KEYWORD, "With"));
                tokenizeVariable(tokens, trim(match[3]));
                tokens.push_back(new Token(TOKEN_KEYWORD, "increment."));
            }
            break;
        case LINE_FOR_BEGIN:
            if (this->getValue() == "Tap Continuous Spell: Magic Formula.") {
                tokens.push_back(new Token(TOKEN_KEYWORD, "Tap Continuous Spell: Magic Formula."));
            }
            break;
        case LINE_FOR_END:
            if (this->getValue() == "End Continuous Spell: Magic Formula.") {
                tokens.push_back(new Token(TOKEN_KEYWORD, "End Continuous Spell: Magic Formula."));
            }
            break;
        case LINE_WHILE:
            tokens.push_back(new Token(TOKEN_KEYWORD, "Activate Continuous Trap: Imperial Order based on previous duel!"));
            break;
        case LINE_WHILE_BEGIN:
            if (this->getValue() == "Check Continuous Trap: Imperial Order!") {
                tokens.push_back(new Token(TOKEN_KEYWORD, "Check Continuous Trap: Imperial Order!"));
            }
            break;
        case LINE_WHILE_END:
            if (this->getValue() == "End Continuous Trap: Imperial Order!") {
                tokens.push_back(new Token(TOKEN_KEYWORD, "End Continuous Trap: Imperial Order!"));
            }
            break;
    }
}