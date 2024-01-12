#ifndef AST_H
#define AST_H

#include <vector>
#include <iostream>

enum class ASTNodeType {
    DECLARATION,
    ASSIGNMENT,
    VARIABLE,
    CONSTANT,
    OPERATION,
    IF,
    ELSE,
    ELSE_IF,
    IF_BODY_START,
    IF_BODY_END,
    ROOT,
    OUTPUT,
    INPUT,
    MAIN_START,
    MAIN_END,
    LOGICAL,
    FOR,
    FOR_BEGIN,
    FOR_END,
    WHILE,
};

class ASTNode {
public:
    ASTNode() {}
    ASTNode(ASTNodeType type) : type(type) {}
    ASTNode(ASTNodeType type, std::string value) : type(type), value(value) {}
    ASTNodeType getType() { return type; }
    void addNode(ASTNode* node) { nodes.push_back(node); }
    void addNode(ASTNode* node, std::string v) { nodes.push_back(node); }
    std::vector<ASTNode*> getNodes() { return nodes; }

    std::string getValue() { return value; }
    void setValue(std::string v) { value = v; }
private:
    ASTNodeType type;
    std::vector<ASTNode*> nodes;
    std::string value;
};

class AST {
public:
    AST() {
        root = new ASTNode(ASTNodeType::ROOT);
    }
    AST(ASTNode* root) : root(root) {}
    ASTNode* getRoot() { return root; }
    void print();
    std::string nodeTypeToString(ASTNodeType type) {
        switch (type) {
            case ASTNodeType::DECLARATION:
                return "Declaration";
            case ASTNodeType::ASSIGNMENT:
                return "Assignment";
            case ASTNodeType::VARIABLE:
                return "Variable";
            case ASTNodeType::CONSTANT:
                return "Constant";
            case ASTNodeType::OPERATION:
                return "Operation";
            case ASTNodeType::IF:
                return "If";
            case ASTNodeType::IF_BODY_START:
                return "Logic Body start";
            case ASTNodeType::IF_BODY_END:
                return "Logic Body end";
            case ASTNodeType::ROOT:
                return "Root";
            case ASTNodeType::OUTPUT:
                return "Output";
            case ASTNodeType::INPUT:
                return "Input";
            case ASTNodeType::MAIN_START:
                return "Main Start";
            case ASTNodeType::MAIN_END:
                return "Main End";
            case ASTNodeType::LOGICAL:
                return "Logical";
            case ASTNodeType::ELSE:
                return "Else";
            case ASTNodeType::ELSE_IF:
                return "Else If";
            case ASTNodeType::FOR:
                return "For";
            case ASTNodeType::FOR_BEGIN:
                return "Loop Begin";
            case ASTNodeType::FOR_END:
                return "Loop End"; 
            default:
                return "Unknown";
        }
    }
private:
    ASTNode* root;
};

#endif // AST_H
