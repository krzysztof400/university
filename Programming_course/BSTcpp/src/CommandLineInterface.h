#include "BinarySearchTree.h"
#include "Parser.h"
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

template <typename T>
class CommandLineInterface {
private:
    std::unique_ptr<Parser<T>> parser;
    BinarySearchTree<T> tree;

public:
    CommandLineInterface(std::unique_ptr<Parser<T>> p) : parser(std::move(p)) {}

    void run() {
        std::string input;
        while (true) {
            std::cout << "Enter command: ";
            std::getline(std::cin, input);

            if (input == "print" || input == "p") {
                tree.print();
                continue;
            }

            std::istringstream ss(input);
            std::string command, value;
            if (std::getline(ss, command, ':') && std::getline(ss, value)) {
                try {
                    T parsedValue = parser->parse(value);
                    if (command == "insert" || command == "i" || command == "add" || command == "a") {
                        tree.insert(parsedValue);
                    } else if (command == "delete" || command == "d" || command == "remove" || command == "r") {
                        tree.deleteNode(parsedValue);
                    } else if (command == "search" || command == "s" || command == "find" || command == "f") {
                        bool found = tree.search(parsedValue);
                        std::cout << "Search result: " << (found ? "Found" : "Not Found") << std::endl;
                    } else {
                        std::cout << "Invalid command" << std::endl;
                    }
                } catch (const std::invalid_argument& e) {
                    std::cout << "Invalid value format: " << value << std::endl;
                }
            } else {
                std::cout << "Invalid command format. Please use 'command: value'." << std::endl;
            }
        }
    }
};
