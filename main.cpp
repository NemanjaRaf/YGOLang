#include <iostream>
#include "lexer.h"
#include "validator.h"
#include "parser.h"

int main() {

    std::cout << "▓██   ██▓ █    ██   ▄████  ██▓ ▒█████   ██░ ██     ██▓    ▄▄▄       ███▄    █   ▄████ " << std::endl;
    std::cout << " ▒██  ██▒ ██  ▓██▒ ██▒ ▀█▒▓██▒▒██▒  ██▒▓██░ ██▒   ▓██▒   ▒████▄     ██ ▀█   █  ██▒ ▀█▒" << std::endl;
    std::cout << "  ▒██ ██░▓██  ▒██░▒██░▄▄▄░▒██▒▒██░  ██▒▒██▀▀██░   ▒██░   ▒██  ▀█▄  ▓██  ▀█ ██▒▒██░▄▄▄░" << std::endl;
    std::cout << "  ░ ▐██▓░▓▓█  ░██░░▓█  ██▓░██░▒██   ██░░▓█ ░██    ▒██░   ░██▄▄▄▄██ ▓██▒  ▐▌██▒░▓█  ██▓" << std::endl;
    std::cout << "  ░ ██▒▓░▒▒█████▓ ░▒▓███▀▒░██░░ ████▓▒░░▓█▒░██▓   ░██████▒▓█   ▓██▒▒██░   ▓██░░▒▓███▀▒" << std::endl;
    std::cout << "   ██▒▒▒ ░▒▓▒ ▒ ▒  ░▒   ▒ ░▓  ░ ▒░▒░▒░  ▒ ░░▒░▒   ░ ▒░▓  ░▒▒   ▓▒█░░ ▒░   ▒ ▒  ░▒   ▒ " << std::endl;
    std::cout << " ▓██ ░▒░ ░░▒░ ░ ░   ░   ░  ▒ ░  ░ ▒ ▒░  ▒ ░▒░ ░   ░ ░ ▒  ░ ▒   ▒▒ ░░ ░░   ░ ▒░  ░   ░ " << std::endl;
    std::cout << " ▒ ▒ ░░   ░░░ ░ ░ ░ ░   ░  ▒ ░░ ░ ░ ▒   ░  ░░ ░     ░ ░    ░   ▒      ░   ░ ░ ░ ░   ░ " << std::endl;
    std::cout << " ░ ░        ░           ░  ░      ░ ░   ░  ░  ░       ░  ░     ░  ░         ░       ░ " << std::endl;
    std::cout << " ░ ░                                                                                  " << std::endl;

    FILE* fp = fopen("test/test.ygo", "r");
    if (!fp) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    Lexer lexer(fp);

    Validator validator(&lexer);
    if (!validator.run()) {
        std::cerr << "Failed to validate" << std::endl;
        return 1;
    }
    Parser parser(&lexer);
    AST* ast = parser.parse();
    ast->print();

    // lexer.printLines();

    fclose(fp);
    std::cout << "Done" << std::endl;

    return 0;
}
