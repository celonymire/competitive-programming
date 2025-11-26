#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Modular integer class for arithmetic modulo M.
 *
 * Automatically maintains values in the range [0, M) and supports all
 * standard arithmetic operations. Uses compile-time constant modulus for
 * efficiency. Division uses Fermat's little theorem for modular inverse,
 * requiring prime modulus.
 *
 * Time Complexity:
 * - All arithmetic operations: O(1)
 * - pow(): O(log P)
 * - inv(): O(log M)
 * Space Complexity: O(1)
 *
 * @tparam M The modulus (must be prime for division to work).
 *
 * @note Modulus must be prime for division and inv() to work correctly.
 * @note The modulus is a compile-time constant for optimization.
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