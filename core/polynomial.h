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

private:
    SingleLinkedList<Monomial> data_;


    void Normalize() {
        data_.Sort();
        SingleLinkedList<Monomial> new_data;
        for (const Monomial &m : data_) {
            if (m.factor == 0) continue;

            if (!new_data.Empty() && Mergeable(new_data.Back(), m)) {
                new_data.Back().factor += m.factor;
            } else {
                new_data.PushBack(m);
            }
        }
        data_ = std::move(new_data);
    }
};

class DivisionResult {
    Polynomial den, num, whole_num;
};
