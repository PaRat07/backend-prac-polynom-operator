#pragma once

#include <algorithm>

#include "single_linked_list.h"
#include "monomial.h"
#include "parser.h"

class Polynomial {
public:
    Polynomial(const std::string &s) {
        Parser parser(s);
        while (!parser.Empty()) {
            data_.PushBack(parser.GetMonomial());
        }
        Normalize();
    }

    Polynomial() = default;

    std::string ToString() const {
        return std::transform_reduce(data_.begin(), data_.end(), std::string(), std::plus<>{}, [] (const Monomial &m) {
            return m.ToString();
        });
    }


    Polynomial operator+(const Polynomial &rhs) const {
        Polynomial ans;
        auto it1 = data_.begin(), it2 = rhs.data_.begin();
        while (it1 != data_.end() && it2 != rhs.data_.end()) {
            if (*it1 < *it2) {
                ans.data_.PushBack(*it1);
                ++it1;
            } else if (*it1 > *it2) {
                ans.data_.PushBack(*it2);
                ++it2;
            } else {
                ans.data_.EmplaceBack(it1->factor + it2->factor, it1->powers);
                ++it1;
                ++it2;
            }
        }
        while (it1 != data_.end()) {
            ans.data_.PushBack(*it1);
            ++it1;
        }
        while (it2 != data_.end()) {
            ans.data_.PushBack(*it2);
            ++it2;
        }
        return ans;
    }

    Polynomial operator-(const Polynomial &rhs) const {
        Polynomial ans;
        std::for_each(rhs.data_.begin(), rhs.data_.end(), [&ans] (const Monomial &m) {
            ans.data_.PushBack(m);
        });
        std::for_each(data_.begin(), data_.end(), [&ans] (const Monomial &m) {
            ans.data_.PushBack(m);
        });
        ans.Normalize();
        return ans;
    }

    Polynomial operator*(const Polynomial &rhs) const {
        Polynomial ans;
        std::for_each(data_.begin(), data_.end(), [&rhs, &ans] (const Monomial &m1) {
            std::for_each(rhs.data_.begin(), rhs.data_.end(), [&ans, &m1] (const Monomial &m2) {
               ans.data_.PushBack(m1 * m2);
            });
        });
        ans.Normalize();
        return ans;
    }

    Polynomial &operator+=(const Polynomial &rhs) {
        return *this = *this + rhs;
    }

    Polynomial &operator-=(const Polynomial &rhs) {
        return *this = *this - rhs;
    }

    Polynomial &operator*=(const Polynomial &rhs) {
        return *this = *this * rhs;
    }

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