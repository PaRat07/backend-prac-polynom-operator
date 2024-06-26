#pragma once

#include <algorithm>
#include <numeric>

#include "linked_list.h"
#include "monomial.h"
#include "parser.h"


class Polynomial {
public:
    Polynomial(const std::string &s);

    Polynomial() = default;

    std::string ToString() const;


    Polynomial operator+(const Polynomial &rhs) const;

    Polynomial operator-(const Polynomial &rhs) const;

    Polynomial operator*(const Polynomial &rhs) const;

    struct DivisionResult;

    DivisionResult operator/(const Polynomial &rhs) const;

    Polynomial &operator+=(const Polynomial &rhs);

    Polynomial &operator-=(const Polynomial &rhs);

    Polynomial &operator*=(const Polynomial &rhs);

    double GetValueAt(const std::array<int, 26> &vals) const;

    LinkedList<int> GetIntegerRoots() const;

    Polynomial GetDerivative(char var) const;

private:
    LinkedList<Monomial> data_;


    void Normalize();
};

struct Polynomial::DivisionResult {
    Polynomial den, num, whole_num;

    std::string ToString() const {
        return whole_num.ToString() + "+\frac{" + num.ToString() + "}{" + den.ToString() + "}";
    }
};

Polynomial operator""_p(const char *data, size_t size);

std::ostream &operator<<(std::ostream &out, const Polynomial &p);
std::ostream &operator<<(std::ostream &out, const Polynomial::DivisionResult &dr);
