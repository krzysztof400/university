#include <string>
#include <stdexcept>

template <typename T>
class Parser {
public:
    virtual T parse(const std::string& input) const = 0;
};

class ParserInteger : public Parser<int> {
public:
    int parse(const std::string& input) const override {
        return std::stoi(input);
    }
};

class ParserDouble : public Parser<double> {
public:
    double parse(const std::string& input) const override {
        return std::stod(input);
    }
};

class ParserString : public Parser<std::string> {
public:
    std::string parse(const std::string& input) const override {
        return input;
    }
};
