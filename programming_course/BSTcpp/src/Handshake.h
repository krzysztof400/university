#include "CommandLineInterface.h"
#include <iostream>
#include <memory>

void handshake() {
    std::cout << "Enter BST type: ";
    std::string bstType;
    std::getline(std::cin, bstType);

    if (bstType == "Integer" || bstType == "Int" || bstType == "int" || bstType == "integer" || bstType == "i" || bstType == "I") {
        auto parser = std::make_unique<ParserInteger>();
        CommandLineInterface<int> cli(std::move(parser));
        cli.run();
    } else if (bstType == "Double" || bstType == "double" || bstType == "d" || bstType == "D") {
        auto parser = std::make_unique<ParserDouble>();
        CommandLineInterface<double> cli(std::move(parser));
        cli.run();
    } else if (bstType == "String" || bstType == "string" || bstType == "s" || bstType == "S") {
        auto parser = std::make_unique<ParserString>();
        CommandLineInterface<std::string> cli(std::move(parser));
        cli.run();
    } else {
        std::cout << "Invalid BST type" << std::endl;
        handshake();
    }
}
