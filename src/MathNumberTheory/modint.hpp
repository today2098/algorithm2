#ifndef ALGORITHM_MODINT_HPP
#define ALGORITHM_MODINT_HPP 1

#include <cassert>
#include <iostream>
#include <utility>  // for swap().

namespace algorithm {

template <int mod>
class Modint {
    long long val;

public:
    // constructor.
    Modint() : Modint(0) {}
    Modint(long long val_) : val(val_) {
        static_assert(mod >= 1);
        if(!(0 <= val and val < mod)) {
            val %= mod;
            if(val < 0) val += mod;
        }
    }

    Modint operator+() const { return Modint(*this); }
    Modint operator-() const { return Modint(0) - (*this); }
    Modint &operator++() {
        val++;
        if(val == mod) val = 0;
        return *this;
    }
    Modint &operator--() {
        if(val == 0) val = mod;
        val--;
        return *this;
    }
    Modint operator++(int) {
        Modint res = *this;
        ++(*this);
        return res;
    }
    Modint operator--(int) {
        Modint res = *this;
        --(*this);
        return res;
    }
    Modint &operator+=(const Modint &a) {
        val += a.val;
        if(val >= mod) val -= mod;
        return *this;
    }
    Modint &operator-=(const Modint &a) {
        val -= a.val;
        if(val < 0) val += mod;
        return *this;
    }
    Modint &operator*=(const Modint &a) {
        val = val * a.val % mod;
        return *this;
    }
    Modint &operator/=(const Modint &a) { return (*this) *= a.inv(); }

    friend Modint operator+(const Modint &x, const Modint &y) { return Modint(x) += y; }
    friend Modint operator-(const Modint &x, const Modint &y) { return Modint(x) -= y; }
    friend Modint operator*(const Modint &x, const Modint &y) { return Modint(x) *= y; }
    friend Modint operator/(const Modint &x, const Modint &y) { return Modint(x) /= y; }
    friend bool operator==(const Modint &x, const Modint &y) { return x.val == y.val; }
    friend bool operator!=(const Modint &x, const Modint &y) { return x.val != y.val; }
    friend std::istream &operator>>(std::istream &is, Modint &x) {
        is >> x.val;
        x.val %= mod;
        if(x.val < 0) x.val += mod;
        return is;
    }
    friend std::ostream &operator<<(std::ostream &os, const Modint &x) { return os << x.val; }

    static constexpr int modulus() { return mod; }
    long long value() const { return val; }
    Modint inv() const {
        long long a = val, b = mod, u = 1, v = 0;
        while(b) {
            long long t = a / b;
            a -= t * b, u -= t * v;
            std::swap(a, b), std::swap(u, v);
        }
        return Modint(u);
    }

    friend Modint mod_pow(const Modint &x, long long k) {
        if(k < 0) return mod_pow(x.inv(), -k);
        Modint res = 1, tmp = x;
        while(k > 0) {
            if(k & 1LL) res *= tmp;
            tmp = tmp * tmp;
            k >>= 1;
        }
        return res;
    }
};

using mint998244353 = Modint<998'244'353>;
using mint1000000007 = Modint<1'000'000'007>;

}  // namespace algorithm

#endif
