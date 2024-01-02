#include "validator.h"
#include "utils.cpp"
#include "line.h"
#include "models/variable.h"
#include <regex>

Validator::Validator(Lexer* lexer) : lexer(lexer) {}

bool Validator::run() {
    parseVariables();
    std::cout << "Loading variables: " << std::endl;
    for (Variable variable : variables) {
        std::cout << "- " << variable << std::endl;
    }
    if (!validate()) {
        return false;
    } else {
        std::cout << "\033[1;32mValid file\033[0;37m" << std::endl;
    }
    return true;
}

bool Validator::isVariableDeclared(std::string name) {
    bool array = false;
    if (name.substr(0, 9) == "Links of ") {
        name = name.substr(9);
        array = true;
    }
    // if starts with "Token with power" it is a constant
    if (name.substr(0, 17) == "Token with power ") {
        return true;
    }

    for (Variable variable : variables) {
        if (variable.getName() == name) {
            if (array) {
                if (variable.getType() == VariableType::VARIABLE_ARRAY) {
                    return true;
                }
            } else {
                if (variable.getType() != VariableType::VARIABLE_ARRAY) {
                    return true;
                }
            }
        }
    }
    return false;
}

Variable Validator::getVariable(std::string name) {
    for (Variable variable : variables) {
        if (variable.getName() == name) {
            return variable;
        }
    }
    return Variable("", "");
}

bool Validator::validate() {
    if (!validateMain()) {
        std::cerr << "Invalid main" << std::endl;
        return false;
    }

    if (!validateLines()) {
        std::cerr << "Invalid line" << std::endl;
        return false;
    }

    if (!validateIfs()) {
        std::cerr << "Invalid IFS" << std::endl;
        return false;
    }

    return true;
}

void Validator::parseVariables() {
    std::cout << "\033[1;37mParsing variables\033[1;0m" << std::endl;
    Node<Line*>* node= lexer->getLines()->getHead();
    if (!node) return;


    Line* line;
    while (node->next) {
        line = node->value;
        node = node->next;
        if (node->value->getType() == LineType::LINE_VARIABLE_DECLARATION) {
            // std::cout << node->value->getValue() << std::endl;
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

bool Validator::validateMain() {
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

bool Validator::validateVariables() {
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

bool Validator::validateIfs() {
     std::cout << "\033[1;37mValidating IFs\033[1;0m" << std::endl;
    Node<Line*>* node = lexer->getLines()->getHead();
    if (!node) return false;

    std::vector<LineType> ifVector;
    int lineCount = 0;

    while (node) {
        lineCount++;
        Line* line = node->value;
        std::cout << *line << std::endl;
        if (line->getType() == LineType::LINE_IF) {
            ifVector.push_back(LineType::LINE_IF);
        } else if (line->getType() == LineType::LINE_IF_START) {
            if (ifVector.empty() || (ifVector.back() != LineType::LINE_IF && ifVector.back() != LineType::LINE_ELSEIF && ifVector.back() != LineType::LINE_ELSE)) {
                std::cout << "Error: Invalid start of logical block at line " << lineCount << std::endl;
                return false;
            }
            ifVector.push_back(LineType::LINE_IF_START);
        } else if (line->getType() == LineType::LINE_IF_END) {
            // check if there is corresponding LINE_IF_START, search backwards, it shoud no be interrupted by another LINE_IF, LINE_ELSEIF or LINE_ELSE 
            bool found = false;
            for (int i = ifVector.size() - 1; i >= 0; i--) {
                if (ifVector[i] == LineType::LINE_IF_START) {
                    found = true;
                    break;
                } else if (ifVector[i] == LineType::LINE_IF || ifVector[i] == LineType::LINE_ELSEIF || ifVector[i] == LineType::LINE_ELSE) {
                    std::cout << "Error: Invalid end of logical block at line " << lineCount << std::endl;
                    return false;
                }
            }
            if (!found) {
                std::cout << "Error: Invalid end of logical block at line " << lineCount << std::endl;
                return false;
            }
            ifVector.push_back(LineType::LINE_IF_END);
        } else if (line->getType() == LineType::LINE_ELSEIF) {
            bool found = false;
            if (std::find(ifVector.begin(), ifVector.end(), LineType::LINE_ELSEIF) != ifVector.end()) {
                std::cout << "Error: There canot be more than one ELSEIF at line " << lineCount << std::endl;
                return false;
            }
            if (ifVector.front() != LineType::LINE_IF) {
                std::cout << "Error: Elseif operaor missing IF at the start " << lineCount << std::endl;
                return false;
            }
            for (int i = ifVector.size() - 1; i >= 0; i--) {
                if (ifVector[i] == LineType::LINE_IF_END) {
                    found = true;
                    break;
                } else if (ifVector[i] == LineType::LINE_IF || ifVector[i] == LineType::LINE_ELSEIF || ifVector[i] == LineType::LINE_ELSE) {
                    std::cout << "Error: Invalid end of logical block at line " << lineCount << std::endl;
                    return false;
                }
            }

            if (!found) {
                std::cout << "Error: Elseif operaor missing IF at the start " << lineCount << std::endl;
                return false;
            }


            ifVector.push_back(LineType::LINE_ELSEIF);
        } else if (line->getType() == LineType::LINE_ELSE) {
            bool found = false;

            if (std::find(ifVector.begin(), ifVector.end(), LineType::LINE_ELSE) != ifVector.end()) {
                std::cout << "Error: There canot be more than one ELSE at line " << lineCount << std::endl;
                return false;
            }
            if (ifVector.front() != LineType::LINE_IF) {
                std::cout << "Error: Else operator missing IF at the start " << lineCount << std::endl;
                return false;
            }
            for (int i = ifVector.size() - 1; i >= 0; i--) {
                if (ifVector[i] == LineType::LINE_IF_END) {
                    found = true;
                    break;
                } else if (ifVector[i] == LineType::LINE_IF || ifVector[i] == LineType::LINE_ELSEIF || ifVector[i] == LineType::LINE_ELSE) {
                    std::cout << "Error: Invalid end of logical block at line " << lineCount << std::endl;
                    return false;
                }
            }


            if (!found) {
                std::cout << "Error: Else operaor missing IF at the start " << lineCount << std::endl;
                return false;
            }

            ifVector.push_back(LineType::LINE_ELSE);
        }

        node = node->next;
    }

    return true;
}


bool Validator::validateLines() {
    std::cout << "\033[1;37mValidating lines\033[1;0m" << std::endl;
    Node<Line*>* node= lexer->getLines()->getHead();
    if (!node) return false;
    Line* line;
    int lineCount = 1;

    while (node->next) {
        lineCount++;
        line = node->value;
        node = node->next;
        if (node->value->getType() == LineType::LINE_VARIABLE_DECLARATION) {
            // std::cout << node->value->getValue() << std::endl;
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
        if (node->value->getType() == LineType::LINE_INPUT_OPERATOR) {
            std::string variableName = node->value->getData()[0];
            if (!isVariableDeclared(variableName)) {
                std::cout << "\033[1;31mVariable " << variableName << " not declared at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
        } else if (node->value->getType() == LineType::LINE_OUTPUT_OPERATOR) {
            std::string variableName = node->value->getData()[0];
            if (!isVariableDeclared(variableName) && variableName.substr(0, 11) != "Spell Card ") {
                std::cout << "\033[1;31mVariable " << variableName << " not declared at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
        } else if (node->value->getType() == LineType::LINE_ARITHMETIC_OPERATOR) {
            // std::cout << node->value->getValue() << std::endl;
            std::string variableName1 = node->value->getData()[0];
            std::string operatorName = node->value->getData()[1];
            std::string variableName2 = node->value->getData()[2];
            if (!isVariableDeclared(variableName1)) {
                std::cout << "\033[1;31mVariable " << variableName1 << " not declared at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
            if (!isVariableDeclared(variableName2)) {
                std::cout << "\033[1;31mVariable " << variableName2 << " not declared at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
            if (getVariable(variableName1).getType() != getVariable(variableName2).getType()) {
                std::cout << "\033[1;31mVariables " << variableName1 << " and " << variableName2 << " are not of the same type at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
        } else if (node->value->getType() == LineType::LINE_LOGICAL_OPERATOR) {
            // std::cout << node->value->getValue() << std::endl;
            std::string variableName1 = node->value->getData()[0];
            std::string variableName2 = node->value->getData()[1];
            std::string operation = node->value->getData()[2];
            if (!isVariableDeclared(variableName1)) {
                std::cout << "\033[1;31mVariable " << variableName1 << " not declared at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
            if (!isVariableDeclared(variableName2)) {
                std::cout << "\033[1;31mVariable " << variableName2 << " not declared at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
            if (getVariable(variableName1).getType() != getVariable(variableName2).getType()) {
                std::cout << "\033[1;31mVariables " << variableName1 << " and " << variableName2 << " are not of the same type at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
        } else if (node->value->getType() == LineType::LINE_FOR) {
            // std::cout << node->value->getValue() << std::endl;
            std::string from = node->value->getData()[0];
            std::string to = node->value->getData()[1];
            std::string increment = node->value->getData()[2];
            if (!isVariableDeclared(from)) {
                std::cout << "\033[1;31mVariable " << from << " not declared at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
            if (!isVariableDeclared(to)) {
                std::cout << "\033[1;31mVariable " << to << " not declared at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
            if (!isVariableDeclared(increment)) {
                std::cout << "\033[1;31mVariable " << increment << " not declared at line " << lineCount << "\033[0;37m" << std::endl;
                return false;
            }
        }


    }

    return true;
}

Validator::~Validator() {
    delete lexer;
}

// Path: lexer.cpp
