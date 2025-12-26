#include "Base.hpp"
#include <cstdlib>
#include <iostream>


Base *generate() {

    int randomNum = rand() % 3;
    if(randomNum == 0)
        return new A;
    else if(randomNum == 1)
        return new B;

    return new C;
}

int main() {
    std::srand(static_cast<unsigned>(std::time(NULL)));

    Base* p = generate();
    std::cout << "generated pointer: " << p << "\n";
    delete p;

    return 0;
}