#include <iostream>
#include "lexer.h"

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
    lexer.printLines();

    std::cout << "Done" << std::endl;

    return 0;
}
