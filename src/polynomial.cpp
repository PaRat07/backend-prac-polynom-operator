#include "../core/polynomial.h"

Polynomial::Polynomial(const std::string &s) {
    Parser parser(s);
    while (!parser.Empty()) {
        data_.PushBack(parser.GetMonomial());
    }
    Normalize();
}

std::string Polynomial::ToString() const {
    return std::transform_reduce(data_.begin(), data_.end(), std::string(), std::plus<>{}, [] (const Monomial &m) {
        return m.ToString();
    });
}

Polynomial Polynomial::operator+(const Polynomial &rhs) const {
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

Polynomial Polynomial::operator-(const Polynomial &rhs) const {
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

Polynomial Polynomial::operator*(const Polynomial &rhs) const {
    Polynomial ans;
    std::for_each(data_.begin(), data_.end(), [&rhs, &ans] (const Monomial &m1) {
        std::for_each(rhs.data_.begin(), rhs.data_.end(), [&ans, &m1] (const Monomial &m2) {
            ans.data_.PushBack(m1 * m2);
        });
    });
    ans.Normalize();
    return ans;
}

Polynomial &Polynomial::operator+=(const Polynomial &rhs) {
    return *this = *this + rhs;
}

Polynomial &Polynomial::operator-=(const Polynomial &rhs) {
    return *this = *this - rhs;
}

Polynomial &Polynomial::operator*=(const Polynomial &rhs) {
    return *this = *this * rhs;
}

DivisionResult Polynomial::operator/(const Polynomial &rhs) const {
    Polynomial this_copy = *this;
    DivisionResult ans;

}

double Polynomial::GetValueAt(const std::array<int, 26> &vals) const {
    return std::transform_reduce(data_.begin(), data_.end(), 0, std::plus<>{}, [&vals] (const Monomial &m) {
        return m.CalcValAt(vals);
    });
}
