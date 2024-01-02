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
    IF_BODY,
    ELSE_BODY,
    ROOT,
    OUTPUT,
    INPUT,
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
private:
    ASTNode* root;
};

#endif // AST_H
