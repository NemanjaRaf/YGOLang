#include "parser.h"

Parser::Parser(Lexer* lexer) : lexer(lexer) {
    ast = new AST();
}

Parser::~Parser() {
    delete ast;
}

AST * Parser::parse() {
    Node<Line*>* node= lexer->getLines()->getHead();
    if (!node) return nullptr;
    Line* line;
    ASTNode* root = ast->getRoot();
    while (node->next) {
        line = node->value;
        if (line->getType() == LineType::LINE_VARIABLE_DECLARATION) {
            ASTNode * leaf = new ASTNode(ASTNodeType::DECLARATION);
            std::string varName = line->getData()[0];
            std::string varValue = line->getData()[2];

            leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, varName));
            leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, varValue));
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_OUTPUT_OPERATOR) {
            ASTNode * leaf = new ASTNode(ASTNodeType::OUTPUT);
            std::string varName = line->getData()[0];
            leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, varName));
            root->addNode(leaf);
        }
        node = node->next;
    }

    return ast;
} 

