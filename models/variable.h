#ifndef VARIABLE_H
#define VARIABLE_H

#pragma once

#include <string>
#include <vector>

enum VariableType {
    VARIABLE_INT,
    VARIABLE_FLOAT,
    VARIABLE_STRING,
    VARIABLE_BOOL,
    VARIABLE_ARRAY,
    VARIABLE_CHAR
};

class Variable {
public:
    Variable(std::string name, std::string type);
    std::string getName() { return name; }
    VariableType getType() { return type; }
    VariableType getArrayType() { return arrayType; }
    bool isArray() { return type == VariableType::VARIABLE_ARRAY || type == VariableType::VARIABLE_STRING; }
    std::vector<std::string> getValue() { return value; }
    void addValue(std::string value) { this->value.push_back(value); }
    friend std::ostream& operator<<(std::ostream& os, const Variable& Line);
    void setArrayType(std::string type) { this->arrayType = stringToVariableType(type); }
    static std::string variableTypeToString(VariableType type);
private:
    std::string name;
    VariableType type;
    VariableType arrayType;
    std::vector<std::string> value;
    VariableType stringToVariableType(std::string type);
};

#endif // VARIABLE_H