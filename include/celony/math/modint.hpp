#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Keeps the internal 32-bit integer value modulo `M`.
 *
 * The modulo is taken as a constant template parameter to optimize for the
 * common case of fixed modulo, and it assumes the modulus is prime.
 *
 * @tparam M The modulus.
 */
template <int M> class modint {
  int v{};

public:
  constexpr modint() = default;
  constexpr modint(long long vx) : v((vx % M + M) % M) {}
  constexpr modint &operator=(long long vx) {
    v = (vx % M + M) % M;
    return *this;
  }

  /**
   * @brief Compute `x^p mod M` using binary exponentiation.
   *
   * @param p The power.
   */
  constexpr modint pow(long long p) const {
    long long a = v, res = 1;
    for (; p; p >>= 1, a = a * a % M)
      if (p & 1)
        res = res * a % M;
    return res;
  }

  /**
   * @brief Compute the modulo inverse of the internal value.
   */
  constexpr modint inv() const { return pow(M - 2); }

  /**
   * @brief Returns the internal integer value.
   */
  constexpr int value() const { return v; }

  constexpr explicit operator int() { return v; }

  constexpr explicit operator bool() { return v != 0; }

#define OP(op, op2, f)                                                         \
  constexpr modint &operator op##=(const modint & other) {                     \
    v = ((1LL * v op2 other.f) % M + M) % M;                                   \
    return *this;                                                              \
  }                                                                            \
  constexpr friend modint operator op(const modint &a, const modint &b) {      \
    auto c = a;                                                                \
    c op## = b;                                                                \
    return c;                                                                  \
  }
  OP(+, +, v)
  OP(-, -, v)
  OP(*, *, v)
  OP(/, *, inv().v)
#undef OP
};