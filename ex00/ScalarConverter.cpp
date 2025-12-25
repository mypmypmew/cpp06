#include "ScalarConverter.hpp"
#include <iostream>
#include <cctype>
#include <string>
#include <cstdlib>
#include <climits>

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
    long v = std::strtol(s.c_str(), 0, 10);

    switch (t)
    {
    case TYPE_CHAR: {
        unsigned char c = static_cast<unsigned char>(s[0]);
        if(!std::isprint(c)) {
            std::cout << "char: " << "Non displayable" << "\n";
        } else {
         std::cout << "char: " <<  "'" << c << "'" << "\n";
        }
        std::cout << "int: " << static_cast<int>(c) << "\n";
        std::cout << "float: " << static_cast<float>(c) << ".0f\n";
        std::cout << "double: " << static_cast<double>(c) << ".0\n";
        break;
    }
    case TYPE_INT: {
        if (v < 0 || v > 127) {
            std::cout << "char: " << "impossible" << "\n";
        } else if(!std::isprint(static_cast<unsigned char>(v))) {
            std::cout << "char: " << "Non displayable" << "\n";
        } else {
             std::cout << "char: '" << static_cast<char>(v) << "'\n";
        }
        if (v >= INT_MIN && v <= INT_MAX) {
            std::cout << "int: " << v << "\n";
        } else {
            std::cout << "int: " << "impossible" << "\n";
        }
        std::cout << "float: " << v << ".0f" << "\n";
        std::cout << "double: " << v << ".0" << "\n";
        break;
    }
    case TYPE_FLOAT:
        break;
    case TYPE_DOUBLE: {
        break;
    }
    case TYPE_PSEUDO_FLOAT:
        std::cout << "char: " << "impossible" << "\n";
        std::cout << "int: " << "impossible" << "\n";
        std::cout << "float: " << s << "\n";
        std::cout << "double: " << s.substr(0, s.size() - 1) << "\n";
        break;    
    case TYPE_PSEUDO_DOUBLE:
        std::cout << "char: " << "impossible" << "\n";
        std::cout << "int: " << "impossible" << "\n";
        std::cout << "float: " << s << "f\n";
        std::cout << "double: " << s << "\n";
        break;
    case TYPE_INVALID:
        std::cout << "char: " << "impossible" << "\n";
        std::cout << "int: " << "impossible" << "\n";
        std::cout << "float: " << "impossible" << "\n";
        std::cout << "double: " << "impossible" << "\n";
        break;
    default:
        break;
    }
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
