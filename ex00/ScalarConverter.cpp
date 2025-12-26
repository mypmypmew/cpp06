#include "ScalarConverter.hpp"
#include <iostream>
#include <cctype>
#include <string>
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <cmath>
#include <limits>
#include <iomanip>



enum LiteralType {
    TYPE_CHAR,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_FLOAT,
    TYPE_PSEUDO_FLOAT,
    TYPE_PSEUDO_DOUBLE,
    TYPE_INVALID
};

static void printImpossible() {
    std::cout << "char: " << "impossible" << "\n";
    std::cout << "int: " << "impossible" << "\n";            
    std::cout << "float: " << "impossible" << "\n";
    std::cout << "double: " << "impossible" << "\n";
}

static void printFixed1(double x) {
    std::ios::fmtflags f = std::cout.flags();
    std::streamsize p = std::cout.precision();
    std::cout << std::fixed << std::setprecision(1) << x;
    std::cout.flags(f);
    std::cout.precision(p);
}

static bool parseLongStrict(const std::string& s, long& out) {
    errno = 0;
    char* end = 0;
    const char* start = s.c_str();
    long v = std::strtol(start, &end, 10);

    if (start == end)
        return false;
    if (*end != '\0')    
        return false;
    if (errno == ERANGE) 
        return false;

    out = v;
    return true;
}

static bool parseDoubleStrict(const std::string& s, double& out) {
    errno = 0;
    char* end = 0;
    const char* start = s.c_str();
    double v = std::strtod(start, &end);

    if (start == end)    
        return false;
    if (*end != '\0')    
        return false;
    if (errno == ERANGE) 
        return false;

    out = v;
    return true;
}

static bool isPseudoDouble(const std::string& s);
static bool isPseudoFloat(const std::string& s);
static bool isCharLiteral(const std::string& s);
static bool isIntLiteral(const std::string& s);
static LiteralType detectType(const std::string& s);

void ScalarConverter::convert(const std::string& s) {
    LiteralType t = detectType(s);

    switch (t)
    {
    case TYPE_CHAR: {
        unsigned char c = static_cast<unsigned char>(s[0]);
        if(!std::isprint(c)) {
            std::cout << "char: " << "Non displayable" << "\n";
        } else {
         std::cout << "char: " <<  "'" << static_cast<char>(c) << "'" << "\n";
        }
        std::cout << "int: " << static_cast<int>(c) << "\n";
        std::cout << "float: " << static_cast<int>(c) << ".0f\n";
        std::cout << "double: " << static_cast<int>(c) << ".0\n";
        break;
    }
    case TYPE_INT: {
        long v;
        if (!parseLongStrict(s, v)) {

        double d;
        if (!parseDoubleStrict(s, d)) {
            printImpossible();
            break;
        }

        std::cout << "char: impossible\n";
        std::cout << "int: impossible\n";

        if (std::fabs(d) > std::numeric_limits<float>::max()) {
            std::cout << "float: impossible\n";
        } else {
            float f = static_cast<float>(d);
            double ip;
            bool intLike = (std::modf(d, &ip) == 0.0);
            std::cout << "float: ";
            if (intLike) {
                printFixed1(static_cast<double>(f));
                std::cout << "f\n";
            } else {
                std::cout << f << "f\n";
            }
        }

        {
            double ip;
            bool intLike = (std::modf(d, &ip) == 0.0);
            std::cout << "double: ";
            if (intLike) {
                printFixed1(d);
                std::cout << "\n";
            } else {
                std::cout << d << "\n";
            }
        }
        break;
    }
        if (v < 0 || v > 127) {
            std::cout << "char: " << "impossible" << "\n";
        } else if(!std::isprint(static_cast<unsigned char>(v))) {
            std::cout << "char: " << "Non displayable" << "\n";
        } else {
             std::cout << "char: '" << static_cast<char>(v) << "'\n";
        }
        if (v >= INT_MIN && v <= INT_MAX) {
            std::cout << "int: " << static_cast<int>(v) << "\n";
        } else {
            std::cout << "int: " << "impossible" << "\n";
        }
        std::cout << "float: ";
        printFixed1(static_cast<double>(static_cast<float>(v)));
        std::cout << "f\n";

        std::cout << "double: ";
        printFixed1(static_cast<double>(v));
        std::cout << "\n";
        break;
    }
    case TYPE_FLOAT: {
        std::string base = s.substr(0, s.size() - 1);
        double d;
        if (!parseDoubleStrict(base, d)) {
            printImpossible();
            break;
        }

        if (d >= static_cast<double>(INT_MIN) && d <= static_cast<double>(INT_MAX)) {
            int fi = static_cast<int>(d);

            if (fi < 0 || fi > 127) {
                std::cout << "char: " << "impossible" << "\n";
            } else if(!std::isprint(static_cast<unsigned char>(fi))) {
                std::cout << "char: " << "Non displayable" << "\n";
            } else {
             std::cout << "char: '" << static_cast<char>(fi) << "'\n";
            }
            std::cout << "int: " << fi << "\n";
        } else {
            std::cout << "char: " << "impossible" << "\n";
            std::cout << "int: " << "impossible" << "\n";
        }

        if (std::fabs(d) > std::numeric_limits<float>::max()) {
            std::cout << "float: impossible\n";
        } else {
            float f = static_cast<float>(d);

            double ip;
            bool isIntLike = (std::modf(d, &ip) == 0.0);

            if (isIntLike) {
                std::cout << "float: ";
                printFixed1(f);
                std::cout << "f\n";
            } else {
                std::cout << "float: " << f << "f\n";
            }
        }

    {
        double ip;
        bool isIntLike = (std::modf(d, &ip) == 0.0);
        if (isIntLike) {
            std::cout << "double: ";
            printFixed1(d);
            std::cout << "\n";
        } else {
            std::cout << "double: " << d << "\n";
        }
    }

    break;

    }
    case TYPE_DOUBLE: {
        double d;
        if (!parseDoubleStrict(s, d)) {
            printImpossible();
            break;
        }

        if (d >= static_cast<double>(INT_MIN) && d <= static_cast<double>(INT_MAX)) {
            int di = static_cast<int>(d);

            if (di < 0 || di > 127) {
                std::cout << "char: impossible\n";
            } else if (!std::isprint(static_cast<unsigned char>(di))) {
                std::cout << "char: Non displayable\n";
            } else {
                std::cout << "char: '" << static_cast<char>(di) << "'\n";
            }
            std::cout << "int: " << di << "\n";
        } else {
            std::cout << "char: impossible\n";
            std::cout << "int: impossible\n";
        }

    if (std::fabs(d) > std::numeric_limits<float>::max()) {
        std::cout << "float: impossible\n";
    } else {
        float f = static_cast<float>(d);
        double ip;
        bool isIntLike = (std::modf(d, &ip) == 0.0);
        if (isIntLike) {
            std::cout << "float: ";
            printFixed1(static_cast<double>(f));
            std::cout << "f\n";
        }
        else         
            std::cout << "float: " << f << "f\n";
    }

    {
        double ip;
        bool isIntLike = (std::modf(d, &ip) == 0.0);
        if (isIntLike) {
            std::cout << "double: ";
            printFixed1(d);
            std::cout << "\n";
        }
        else          
        std::cout << "double: " << d << "\n";
    }
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
