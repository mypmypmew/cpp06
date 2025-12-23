#include "ScalarConverter.hpp"
#include <iostream>
#include <cctype>
#include <string>

enum LiteralType {
    TYPE_CHAR,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_FLOAT,
    TYPE_PSEUDO_FLOAT,
    TYPE_PSEUDO_DOUBLE,
    TYPE_INVALID
};

static bool isPseudoDouble(const std::string& s);
static bool isPseudoFloat(const std::string& s);
static bool isCharLiteral(const std::string& s);
static bool isIntLiteral(const std::string& s);
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

static bool isIntLiteral(const std::string& s) {
    std::size_t i = 0;
    if (s.empty())
        return false;
    if (s[0] == '-' || s[0] == '+')
        i++;
    if (i == s.size())
        return false;
    for (; i < s.size(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
            return false;
        }
    }
    return true;
}

static bool isDoubleLiteral(const std::string& s) {
    std::size_t i = 0;
    int dotCount = 0;
    bool hasDigit = false;

    if (s.empty())
        return false;
    if (s[0] == '-' || s[0] == '+')
        i++;
    if (i == s.size())
        return false;
    for (; i < s.size(); i++) {
        if (std::isdigit(static_cast<unsigned char>(s[i]))) {
            hasDigit = true;
        } else {
            if (s[i] == '.') {
                dotCount++;
            } else {
                return false;
            }
        }
    }
    if (dotCount == 1 && hasDigit)
        return true;
    else 
        return false;
}

static bool isFloatLiteral(const std::string& s) {
    if (s.size() < 2)
        return false;
    if (s[s.size() - 1] != 'f')
        return false;
    if (isIntLiteral(s.substr(0, s.size() - 1)))
        return true;
    if (isDoubleLiteral(s.substr(0, s.size() - 1)))
        return true;
    return false;
}

static LiteralType detectType(const std::string& s) {
    if (isPseudoDouble(s)) 
        return TYPE_PSEUDO_DOUBLE;
    if (isPseudoFloat(s))  
        return TYPE_PSEUDO_FLOAT;
    if (isCharLiteral(s))  
        return TYPE_CHAR;
    if (isIntLiteral(s))
        return TYPE_INT; 
    if (isDoubleLiteral(s))
        return TYPE_DOUBLE;
    if (isFloatLiteral(s))
        return TYPE_FLOAT;   
    return TYPE_INVALID;
}
