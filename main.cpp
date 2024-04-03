#include <iostream>

//#include "extern/cpp-httplib/httplib.h"

#include "core/polynomial.h"

int main() {
//    Polynomial p1("x+1"), p2("x-2"), p3("x-5");
//    Polynomial p = p1 * p2 * p3;
//    Polynomial::DivisionResult pdivp2 = p / Polynomial();
//    std::cout << pdivp2.whole_num.ToString() << " + (" << pdivp2.num.ToString() << ") / (" << pdivp2.den.ToString() << ")" << std::endl;
    std::cout << ("x^2+2x+1"_p / "x+1"_p).whole_num << std::endl;
}