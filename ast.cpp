#include "ast.h"
#include <iostream>

void AST::print() {
    std::cout << "AST:" << std::endl;
    // print tree
    std::vector<ASTNode*> nodes = root->getNodes();
    for (auto node : nodes) {
        if (node->getType() == ASTNodeType::OUTPUT || node->getType() == ASTNodeType::INPUT) {
            ASTNode * leaf = node->getNodes()[0];
            if (leaf->getType() == ASTNodeType::VARIABLE) {
                std::cout << "----------NODE----------" << std::endl;
                std::cout << nodeTypeToString(node->getType()) << std::endl;
                std::cout << "Variable: " << leaf->getValue() << std::endl;
                std::cout << "------------------------" << std::endl;
            } else {
                std::cout << "----------NODE----------" << std::endl;
                std::cout << nodeTypeToString(node->getType()) << std::endl;
                std::cout << "Constant: " << leaf->getValue() << std::endl;
                std::cout << "------------------------" << std::endl;
            }
        } else if (node->getType() == ASTNodeType::DECLARATION) {
            ASTNode * leaf = node->getNodes()[0];
            if (leaf->getType() == ASTNodeType::VARIABLE) {
                std::cout << "----------NODE----------" << std::endl;
                std::cout << nodeTypeToString(node->getType()) << std::endl;
                std::cout << "Declarator: " << leaf->getValue() << std::endl;
                std::cout << "Constant: " << node->getNodes()[1]->getValue() << std::endl;
                std::cout << "------------------------" << std::endl;
            }
        } else if (node->getType() == ASTNodeType::LOGICAL || node->getType() == ASTNodeType::ELSE_IF) {
            std::cout << "----------NODE----------" << std::endl;
            std::cout << nodeTypeToString(node->getType()) << std::endl;
            std::cout << "     -----------------------------------" << std::endl;
            std::cout << "     |                |                |" << std::endl;
            std::cout << "     " << node->getNodes()[0]->getValue() << "                " << node->getNodes()[1]->getValue() << "                " << node->getNodes()[2]->getValue() << std::endl;
            std::cout << "     " << nodeTypeToString(node->getNodes()[0]->getType()) << "                " << nodeTypeToString(node->getNodes()[1]->getType()) << "                " << nodeTypeToString(node->getNodes()[2]->getType()) << std::endl;
            std::cout << "------------------------" << std::endl;
        } else if (node->getType() == ASTNodeType::VARIABLE) {
            std::cout << "----------NODE----------" << std::endl;
            std::cout << "Assign" << std::endl;
            std::cout << "Variable: " << node->getValue() << std::endl;
            std::cout << "     -----------------------------------" << std::endl;
            std::cout << "     |                |                |" << std::endl;
            std::cout << "     " << node->getValue() << "                " << node->getNodes()[0]->getValue() << "                " << node->getNodes()[1]->getValue() << std::endl;
            std::cout << "     " << nodeTypeToString(node->getType()) << "                " << nodeTypeToString(node->getNodes()[0]->getType()) << "                " << nodeTypeToString(node->getNodes()[1]->getType()) << std::endl;
            std::cout << "------------------------" << std::endl;
        } else {
            std::cout << "----------NODE----------" << std::endl;
            std::cout << nodeTypeToString(node->getType()) << std::endl;
            std::cout << "------------------------" << std::endl;
        }

    }
}