#ifndef ALGORITHM_IGCD_ILCM_HPP
#define ALGORITHM_IGCD_ILCM_HPP 1

namespace algorithm {

// 最大公約数 (GCD:Greatest Common Divisor)．O(logN).
template <typename Type>
Type igcd(Type a, Type b) { return (b == 0) ? a : igcd(b, a % b); }

// 最小公倍数 (LCM:Least Common Multiple)．O(logN).
template <typename Type>
Type ilcm(Type a, Type b) { return a / igcd(a, b) * b; }

}  // namespace algorithm

#endif
