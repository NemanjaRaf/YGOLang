#include "parser.h"
#include "utils.cpp"
#include "line.h"
#include "models/variable.h"
#include <regex>

Parser::Parser(Lexer* lexer) : lexer(lexer) {}

void Parser::parse() {
    parseVariables();
    std::cout << "Variables: " << std::endl;
    for (Variable variable : variables) {
        std::cout << "- " << variable << std::endl;
    }
    std::cout << "Parsing" << std::endl;
    if (!validate()) {
        return;
    } else {
        std::cout << "\033[1;32mValid file\033[0;37m" << std::endl;
    }
}

bool Parser::isVariableDeclared(std::string name) {
    for (Variable variable : variables) {
        if (variable.getName() == name) {
            return true;
        }
    }
    return false;
}

bool Parser::validate() {
    if (!validateMain()) {
        std::cerr << "Invalid main" << std::endl;
        return false;
    }

    if (!validateVariables()) {
        std::cerr << "Invalid variables" << std::endl;
        return false;
    }

    return true;
}

void Parser::parseVariables() {
    std::cout << "\033[1;37mParsing variables\033[1;0m" << std::endl;
    Node<Line*>* node= lexer->getLines()->getHead();
    if (!node) return;


    Line* line;
    while (node->next) {
        line = node->value;
        node = node->next;
        if (node->value->getType() == LineType::LINE_VARIABLE_DECLARATION) {
            std::cout << node->value->getValue() << std::endl;
            Variable var = Variable(node->value->getData()[0], node->value->getData()[1]);
            if (node->value->getData()[3] != "") {
                var.setArrayType(node->value->getData()[3]);
                std::vector<std::string> values = split(node->value->getData()[4], ',');
                for (std::string value : values) {
                    var.addValue(value);
                }
            } else {
                var.addValue(node->value->getData()[2]);
            }
            addVariable(var);
        }
    }
    std::cout << "Done parsing variables" << std::endl;
}

bool Parser::validateMain() {
    std::cout << "Validating main" << std::endl;
    DoubleLinkedList<Line*> * lines = lexer->getLines();
    Node<Line*>* node = lines->getHead();
    if (!node) {
        return false;
    }
    if (node->value->getType() != LineType::LINE_START_MAIN) {
        return false;
    }
    Line* line;
    while (node->next) {
        line = node->value;
        node = node->next;
    }
    line = node->value;
    if (line->getType() != LineType::LINE_END_MAIN) {
        return false;
    }

    return true;
}

bool Parser::validateVariables() {
    std::cout << "Validating variables" << std::endl;
    for (Variable var : variables) {
        if (var.getType() == VariableType::VARIABLE_ARRAY) {
            if (var.getArrayType() == VariableType::VARIABLE_INT) {
                for (std::string value : var.getValue()) {
                    if (!std::regex_match(value, std::regex("^[0-9]+$"))) {
                        std::cout << "\033[1;31mInvalid int value: " << value << " \033[0;37m;" << std::endl;
                        return false;
                    }
                }
            } else if (var.getArrayType() == VariableType::VARIABLE_FLOAT) {
                for (std::string value : var.getValue()) {
                    if (!std::regex_match(value, std::regex("^[0-9]+\\.[0-9]+$"))) {
                        std::cout << "\033[1;31mInvalid float value: " << value << " \033[0;37m;" << std::endl;
                        return false;
                    }
                }
            } else if (var.getArrayType() == VariableType::VARIABLE_STRING) {
                for (std::string value : var.getValue()) {
                    if (!std::regex_match(value, std::regex("^\".*\"$"))) {
                        std::cout << "\033[1;31mInvalid string value: " << value << " \033[0;37m;" << std::endl;
                        return false;
                    }
                }
            } else if (var.getArrayType() == VariableType::VARIABLE_CHAR) {
                for (std::string value : var.getValue()) {
                    if (!std::regex_match(value, std::regex("^'.'$"))) {
                        std::cout << "\033[1;31mInvalid char value: " << value << " \033[0;37m;" << std::endl;
                        return false;
                    }
                }
            }
        } else {
            if (var.getType() == VariableType::VARIABLE_INT) {
                if (!std::regex_match(var.getValue()[0], std::regex("^[0-9]+$"))) {
                    std::cout << "\033[1;31mInvalid int value: " << var.getValue()[0] << " \033[0;37m;" << std::endl;
                    return false;
                }
            } else if (var.getType() == VariableType::VARIABLE_FLOAT) {
                if (!std::regex_match(var.getValue()[0], std::regex("^[0-9]+\\.[0-9]+$"))) {
                    std::cout << "\033[1;31mInvalid float value: " << var.getValue()[0] << " \033[0;37m;" << std::endl;
                    return false;
                }
            } else if (var.getType() == VariableType::VARIABLE_STRING) {
                if (!std::regex_match(var.getValue()[0], std::regex("^\".*\"$"))) {
                    std::cout << "\033[1;31mInvalid string value: " << var.getValue()[0] << " \033[0;37m;" << std::endl;
                    return false;
                }
            } else if (var.getType() == VariableType::VARIABLE_CHAR) {
                if (!std::regex_match(var.getValue()[0], std::regex("^'.'$"))) {
                    std::cout << "\033[1;31mInvalid char value: " << var.getValue()[0] << " \033[0;37m;" << std::endl;
                    return false;
                }
            }
        }
    }
    return true;
}

Parser::~Parser() {
    delete lexer;
}

// Path: lexer.cpp
