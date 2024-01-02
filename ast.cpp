#include "ast.h"
#include <iostream>

void AST::print() {
    std::cout << "AST:" << std::endl;
    // print tree
    std::vector<ASTNode*> nodes = root->getNodes();
    for (auto node : nodes) {
        if (node->getType() == ASTNodeType::DECLARATION) {
            ASTNode * leaf = node->getNodes()[0];
            if (leaf->getType() == ASTNodeType::VARIABLE) {
                std::cout << "------------------------" << std::endl;
                std::cout << "Declaration" << std::endl;
                std::cout << "Declarator: " << leaf->getValue() << std::endl;
                std::cout << "Constant: " << node->getNodes()[1]->getValue() << std::endl;
                std::cout << "------------------------" << std::endl;
            }
        }

    }
}