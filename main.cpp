#include <iostream>

#include "extern/cpp-httplib/httplib.h"

#include "core/polynomial.h"

int main() {
    Polynomial p("2x+1");
    p *= p;
    std::cout << p.ToString() << std::endl;
}