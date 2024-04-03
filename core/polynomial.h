#pragma once

#include <algorithm>
#include <numeric>

#include "single_linked_list.h"
#include "monomial.h"
#include "parser.h"

class DivisionResult;

class Polynomial {
public:
    Polynomial(const std::string &s);

    Polynomial() = default;

    std::string ToString() const;


    Polynomial operator+(const Polynomial &rhs) const;

    Polynomial operator-(const Polynomial &rhs) const;

    Polynomial operator*(const Polynomial &rhs) const;

    DivisionResult operator/(const Polynomial &rhs) const;

    Polynomial &operator+=(const Polynomial &rhs);

    Polynomial &operator-=(const Polynomial &rhs);

    Polynomial &operator*=(const Polynomial &rhs);

    double GetValueAt(const std::array<int, 26> &vals) const;

    SingleLinkedList<int> GetIntegerRoots() const;

//    SingleLinkedList<int>

    Polynomial GetDerivative() const {
        SingleLinkedList<Monomial> ans;
        std::transform(data_.begin(), data_.end(), std::back_inserter(ans), [] (const Monomial &m) {

        });
    }

private:
    SingleLinkedList<Monomial> data_;


    void Normalize();
};

class DivisionResult {
    Polynomial den, num, whole_num;
};
