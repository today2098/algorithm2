#ifndef ALGORITHM_DYNAMIC_MODINT_HPP
#define ALGORITHM_DYNAMIC_MODINT_HPP 1

#include <cassert>
#include <iostream>
#include <utility>  // for swap().

namespace algorithm {

template <int id>
class DynamicModint {
    static int mod;
    long long val;

public:
    // constructor.
    DynamicModint() : DynamicModint(0) {}
    DynamicModint(long long val_) : val(val_) {
        assert(mod >= 1);
        val %= mod;
        if(val < 0) val += mod;
    }

    DynamicModint operator+() const { return DynamicModint(*this); }
    DynamicModint operator-() const { return DynamicModint(0) - (*this); }
    DynamicModint &operator++() {
        val++;
        if(val == mod) val = 0;
        return *this;
    }
    DynamicModint &operator--() {
        if(val == 0) val = mod;
        val--;
        return *this;
    }
    DynamicModint operator++(int) {
        DynamicModint res = *this;
        ++(*this);
        return res;
    }
    DynamicModint operator--(int) {
        DynamicModint res = *this;
        --(*this);
        return res;
    }
    DynamicModint &operator+=(const DynamicModint &a) {
        val += a.val;
        if(val >= mod) val -= mod;
        return *this;
    }
    DynamicModint &operator-=(const DynamicModint &a) {
        val -= a.val;
        if(val < 0) val += mod;
        return *this;
    }
    DynamicModint &operator*=(const DynamicModint &a) {
        val = val * a.val % mod;
        return *this;
    }
    DynamicModint &operator/=(const DynamicModint &a) { return (*this) *= a.inv(); }

    friend DynamicModint operator+(const DynamicModint &x, const DynamicModint &y) { return DynamicModint(x) += y; }
    friend DynamicModint operator-(const DynamicModint &x, const DynamicModint &y) { return DynamicModint(x) -= y; }
    friend DynamicModint operator*(const DynamicModint &x, const DynamicModint &y) { return DynamicModint(x) *= y; }
    friend DynamicModint operator/(const DynamicModint &x, const DynamicModint &y) { return DynamicModint(x) /= y; }
    friend bool operator==(const DynamicModint &x, const DynamicModint &y) { return x.val == y.val; }
    friend bool operator!=(const DynamicModint &x, const DynamicModint &y) { return x.val != y.val; }
    friend std::istream &operator>>(std::istream &is, DynamicModint &x) {
        is >> x.val;
        x.val %= mod;
        if(x.val < 0) x.val += mod;
        return is;
    }
    friend std::ostream &operator<<(std::ostream &os, const DynamicModint &x) { return os << x.val; }

    static constexpr int get_id() { return id; }
    static void set_modulus(int mod_) {
        assert(mod_ >= 1);
        mod = mod_;
    }
    static int modulus() { return mod; }
    long long value() const { return val; }
    DynamicModint inv() const {
        long long a = val, b = mod, u = 1, v = 0;
        while(b) {
            long long t = a / b;
            a -= t * b, u -= t * v;
            std::swap(a, b), std::swap(u, v);
        }
        return DynamicModint(u);
    }

    friend DynamicModint mod_pow(const DynamicModint &x, long long k) {
        if(k < 0) return mod_pow(x.inv(), -k);
        DynamicModint res = 1, tmp = x;
        while(k > 0) {
            if(k & 1LL) res *= tmp;
            tmp = tmp * tmp;
            k >>= 1;
        }
        return res;
    }
};

template <int id>
int DynamicModint<id>::mod = 1'000'000'007;

}  // namespace algorithm

#endif
