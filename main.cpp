#include <iostream>

//#include "extern/cpp-httplib/httplib.h"

#include "core/polynomial.h"

int main() {
    Polynomial p1("x+1"), p2("x-2"), p3("x-5");
    Polynomial p = p1 * p2 * p3;
    SingleLinkedList<int> l;
    auto it = l.cbegin();
    it++;
    std::array<int, 26> arr;
    arr.fill(0);
    arr['x' - 'a'] = 1;
    std::cout << p.GetValueAt(arr) << std::endl;
    std::cout << p.ToString() << std::endl;
    for (auto i : p.GetIntegerRoots()) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}