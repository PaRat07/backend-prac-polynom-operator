#pragma once

#include <algorithm>
#include <array>
#include <string>

struct Monomial {
    Monomial()
        : factor(0)
    {
        powers.fill(0);
    }

    Monomial(const int &f, const std::array<int, 26> &p)
        : factor(f)
        , powers(p)
    {}

    int factor;
    std::array<int, 26> powers;


    auto operator<=>(const Monomial &rhs) const {
        return std::lexicographical_compare_three_way(powers.begin(), powers.end(), rhs.powers.begin(), rhs.powers.end());
    }

    Monomial operator-() const {
        return { -factor, powers };
    }

    Monomial operator*(Monomial rhs) const {
        rhs.factor *= factor;
        std::transform(rhs.powers.begin(), rhs.powers.end(), powers.begin(), rhs.powers.begin(), std::plus<>{});
        return rhs;
    }

    bool operator==(const Monomial &rhs) const {
        return std::equal(powers.begin(), powers.end(), rhs.powers.begin());
    }

    bool operator!=(const Monomial &rhs) const {
        return !(*this == rhs);
    }

    std::string ToString() const {
        std::string ans;
        if (factor >= 0) {
            ans.push_back('+');
        }
        ans += std::to_string(factor);
        for (int i = 0; i < 26; ++i) {
            if (powers[i] > 0) {
                ans.push_back('a' + i);
                if (powers[i] > 1) {
                    ans.push_back('^');
                    ans += std::to_string(powers[i]);
                }
            }
        }
        return ans;
    }
};

bool Mergeable(const Monomial &a, const Monomial &b) {
    return a == b;
}