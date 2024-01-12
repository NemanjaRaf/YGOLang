#include "parser.h"
#include "utils.cpp"

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
            if (varName.substr(0, 11) == "Spell Card ") {
                varName = varName.substr(11, varName.length());
                leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, varName));
            } else {
                leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, varName));
            }
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_INPUT_OPERATOR) {
            ASTNode * leaf = new ASTNode(ASTNodeType::INPUT);
            std::string varName = line->getData()[0];
            leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, varName));
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_START_MAIN) {
            ASTNode * leaf = new ASTNode(ASTNodeType::MAIN_START);
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_END_MAIN) {
            ASTNode * leaf = new ASTNode(ASTNodeType::MAIN_END);
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_IF_START) {
            ASTNode * leaf = new ASTNode(ASTNodeType::IF_BODY_START);
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_IF_END) {
            ASTNode * leaf = new ASTNode(ASTNodeType::IF_BODY_END);
            root->addNode(leaf);
        }

        if (line->getType() == LineType::LINE_LOGICAL_OPERATOR) {
            ASTNode * leaf = new ASTNode(ASTNodeType::LOGICAL);
            std::string var1 = line->getData()[0];
            std::string var2 = line->getData()[1];
            std::string op = line->getData()[2];
            
            if (var1.substr(0, 17) == "Token with power ") {
                var1 = var1.substr(17, var1.length());
                leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, var1));
            } else {
                leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, var1));
            }

            if (op == "Equality") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "=="));
            } else if (op == "Difference") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "!="));
            } else if (op == "Dominance") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "<"));
            } else if (op == "Subservience") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "<="));
            } else if (op == "Supremacy") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, ">"));
            } else if (op == "Ascendancy") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, ">="));
            } else {
                std::cerr << "Unknown logical operator: " << op << std::endl;
                return nullptr;
            }

            if (var2.substr(0, 17) == "Token with power ") {
                var2 = var2.substr(17, var2.length());
                leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, var2));
            } else {
                leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, var2));
            }

            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_ARITHMETIC_OPERATOR) {
            std::string var1 = line->getData()[0];
            std::string var2 = trim(line->getData()[2]);
            std::string op = line->getData()[1];

            ASTNode * leaf = new ASTNode(ASTNodeType::VARIABLE, var1);

            if (op == "Attacks") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "+"));
            } else if (op == "Defends") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "-"));
            } else if (op == "Combines with") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "*"));
            } else if (op == "Splits by") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "/"));
            } else if (op == "Duels") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "%"));
            } else if (op == "Copies") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "="));
            } else {
                std::cerr << "Unknown arithmetic operator: " << op << std::endl;
                return nullptr;
            }

            if (var2.substr(0, 17) == "Token with power ") {
                var2 = var2.substr(17, var2.length());
                leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, var2));
            } else {
                leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, var2));
            }


            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_IF) {
            ASTNode * leaf = new ASTNode(ASTNodeType::IF);
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_ELSE) {
            ASTNode * leaf = new ASTNode(ASTNodeType::ELSE);
            root->addNode(leaf);
        }
        // else_if same as logical
        if (line->getType() == LineType::LINE_ELSEIF) {
            ASTNode * leaf = new ASTNode(ASTNodeType::ELSE_IF);
            std::string var1 = line->getData()[0];
            std::string var2 = line->getData()[1];
            std::string op = line->getData()[2];
            
            if (var1.substr(0, 17) == "Token with power ") {
                var1 = var1.substr(17, var1.length());
                leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, var1));
            } else {
                leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, var1));
            }

            if (op == "Equality") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "=="));
            } else if (op == "Difference") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "!="));
            } else if (op == "Dominance") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "<"));
            } else if (op == "Subservience") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, "<="));
            } else if (op == "Supremacy") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, ">"));
            } else if (op == "Ascendancy") {
                leaf->addNode(new ASTNode(ASTNodeType::OPERATION, ">="));
            } else {
                std::cerr << "Unknown logical operator: " << op << std::endl;
                return nullptr;
            }

            if (var2.substr(0, 17) == "Token with power ") {
                var2 = var2.substr(17, var2.length());
                leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, var2));
            } else {
                leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, var2));
            }

            root->addNode(leaf);
        }

        if (line->getType() == LineType::LINE_FOR) {
            ASTNode * leaf = new ASTNode(ASTNodeType::FOR);
            std::string var1 = line->getData()[0];
            std::string var2 = line->getData()[1];
            std::string var3 = line->getData()[2];

            if (var1.substr(0, 17) == "Token with power ") {
                var1 = var1.substr(17, var1.length());
                leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, var1));
            } else {
                leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, var1));
            }

            if (var2.substr(0, 17) == "Token with power ") {
                var2 = var2.substr(17, var2.length());
                leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, var2));
            } else {
                leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, var2));
            }

            if (var3.substr(0, 17) == "Token with power ") {
                var3 = var3.substr(17, var3.length());
                leaf->addNode(new ASTNode(ASTNodeType::CONSTANT, var3));
            } else {
                leaf->addNode(new ASTNode(ASTNodeType::VARIABLE, var3));
            }
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_FOR_BEGIN || line->getType() == LineType::LINE_WHILE_BEGIN) {
            ASTNode * leaf = new ASTNode(ASTNodeType::FOR_BEGIN);
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_FOR_END || line->getType() == LineType::LINE_WHILE_END) {
            ASTNode * leaf = new ASTNode(ASTNodeType::FOR_END);
            root->addNode(leaf);
        }
        if (line->getType() == LineType::LINE_WHILE) {
            ASTNode * leaf = new ASTNode(ASTNodeType::WHILE);
            root->addNode(leaf);
        }
        node = node->next;
    }

    return ast;
} 

