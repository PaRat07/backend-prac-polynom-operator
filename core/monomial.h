#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <numeric>
#include <cmath>
#include <sstream>

struct Monomial {
    double factor;
    std::array<int, 26> powers;

    Monomial();
    Monomial(const double &f, const std::array<int, 26> &p);


    std::strong_ordering operator<=>(const Monomial &rhs) const;
    bool operator==(const Monomial &rhs) const;
    bool operator!=(const Monomial &rhs) const;

    Monomial operator-() const;

    Monomial operator*(Monomial rhs) const;

    std::string ToString() const;

    double CalcValAt(const std::array<int, 26> &arr) const;

    Monomial GetDerivative(int letter) const {
        if (powers[letter] == 0) return {};
        Monomial ans = *this;
        ans.factor *= ans.powers[letter]--;
        return ans;
    }
};

bool Mergeable(const Monomial &a, const Monomial &b);
