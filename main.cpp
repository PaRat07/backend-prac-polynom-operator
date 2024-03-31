#include <bits/stdc++.h>
#include "core/single_linked_list.h"

#include "extern/cpp-httplib/httplib.h"

#include "core/polynomial.h"

int main() {
    Polynomial pol("-27a^3b+132xy");
    std::cout << pol.ToString() << std::endl;
}