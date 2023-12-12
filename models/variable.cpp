#include "variable.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Variable& variable) {
    os << variable.name << " (" << Variable::variableTypeToString(variable.type) << ") (";
    if (!variable.value.empty()) {
        for (auto it = variable.value.begin(); it != std::prev(variable.value.end()); ++it) {
            os << *it << ", ";
        }
        os << variable.value.back();
    }
    os << ")" << std::endl;
    return os;
}

Variable::Variable(std::string name, std::string type) {
    this->name = name;
    this->type = stringToVariableType(type);
}

VariableType Variable::stringToVariableType(std::string type) {
    if (type == "Normal Monster") {
        return VariableType::VARIABLE_INT;
    } else if (type == "Effect Monster") {
        return VariableType::VARIABLE_FLOAT;
    } else if (type == "Equip Spell") {
        return VariableType::VARIABLE_BOOL;
    } else if (type == "Link Monster") {
        return VariableType::VARIABLE_ARRAY;
    } else if (type == "Tuner Monster") {
        return VariableType::VARIABLE_CHAR;
    } else if (type == "Fusion Monster") {
        return VariableType::VARIABLE_STRING;
    }
    return VariableType::VARIABLE_INT;
}

std::string Variable::variableTypeToString(VariableType type) {
    switch (type) {
        case VariableType::VARIABLE_INT:
            return "Normal Monster";
        case VariableType::VARIABLE_FLOAT:
            return "Effect Monster";
        case VariableType::VARIABLE_BOOL:
            return "Equip Spell";
        case VariableType::VARIABLE_ARRAY:
            return "Link Monster";
        case VariableType::VARIABLE_CHAR:
            return "Tuner Monster";
        case VariableType::VARIABLE_STRING:
            return "Fusion Monster";
    }
    return "Unknown Monster";
}
