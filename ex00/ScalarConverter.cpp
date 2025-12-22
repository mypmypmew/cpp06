#include "ScalarConverter.hpp"
#include <iostream>
#include <cctype>
#include <string>

enum LiteralType {
    TYPE_CHAR,
    TYPE_PSEUDO_FLOAT,
    TYPE_PSEUDO_DOUBLE,
    TYPE_INVALID
};

static bool isPseudoDouble(const std::string& s);
static bool isPseudoFloat(const std::string& s);
static bool isCharLiteral(const std::string& s);
static LiteralType detectType(const std::string& s);

void ScalarConverter::convert(const std::string& s) {
    LiteralType t = detectType(s);
    std::cout << "detected=" << t << "\n";
}

static bool isPseudoDouble(const std::string& s) {
    return (s == "nan" || s == "+inf" || s == "-inf");
}

static bool isPseudoFloat(const std::string& s) {
    return (s == "nanf" || s == "+inff" || s == "-inff");
}

static bool isCharLiteral(const std::string& s) {
    return (s.size() == 1 && !std::isdigit(static_cast<unsigned char>(s[0])));
}

static LiteralType detectType(const std::string& s) {
    if (isPseudoDouble(s)) return TYPE_PSEUDO_DOUBLE;
    if (isPseudoFloat(s))  return TYPE_PSEUDO_FLOAT;
    if (isCharLiteral(s))  return TYPE_CHAR;
    return TYPE_INVALID;
}
