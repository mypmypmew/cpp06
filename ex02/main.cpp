#include "Base.hpp"
#include <cstdlib>
#include <iostream>
#include <ctime>


Base *generate() {
    int randomNum = rand() % 3;
    if(randomNum == 0)
        return new A;
    else if(randomNum == 1)
        return new B;

    return new C;
}

void identify(Base& p) {
    try {
        (void)dynamic_cast<A&>(p);
        std::cout << "A\n";
        return;
    } catch (...) {}

    try {
        (void)dynamic_cast<B&>(p);
        std::cout << "B\n";
        return;
    } catch (...) {}

    try {
        (void)dynamic_cast<C&>(p);
        std::cout << "C\n";
        return;
    } catch (...) {}

    std::cout << "Unknown\n";
}


void identify(Base* p) {
    if (dynamic_cast<A*>(p))
        std::cout << "A\n";
    else if (dynamic_cast<B*>(p))
        std::cout << "B\n";
    else if (dynamic_cast<C*>(p))
        std::cout << "C\n";
    else
        std::cout << "Unknown\n";
}

int main() {
    std::srand(static_cast<unsigned>(std::time(NULL)));

    Base* p = generate();
    identify(p);
    identify(*p);
    delete p;

    return 0;
}