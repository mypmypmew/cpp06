#include <iostream>
#include "Serializer.hpp"
#include "Data.hpp"

int main() {
    Data data;
    data.n = 42;
    data.s = "hello";

    Data* p = &data;

    uintptr_t raw = Serializer::serialize(p);
    Data* q = Serializer::deserialize(raw);

    std::cout << "p: " << p << "\n";
    std::cout << "raw: " << raw << "\n";
    std::cout << "q: " << q << "\n";

    std::cout << "q->n: " << q->n << "\n";
    std::cout << "q->s: " << q->s << "\n";

    if (q == p)
        std::cout << "OK: pointers match\n";
    else
        std::cout << "KO: pointers differ\n";

    return 0;
}