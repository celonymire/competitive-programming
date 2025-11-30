#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Matrix class optimized for competitive programming.
 *
 * Supports basic matrix operations: addition, subtraction, multiplication,
 * and fast exponentiation. Designed for simplicity and efficiency in
 * contest environments, particularly for solving linear recurrences and
 * graph problems.
 *
 * Time Complexity:
 * - Addition/Subtraction: \f$O(N \cdot M)\f$
 * - Multiplication: \f$O(N \cdot C \cdot M)\f$ where C is the common dimension
 * - Power: \f$O(N^3 \log P)\f$ for N×N matrices
 * Space Complexity: \f$O(N \cdot M)\f$
 *
 * @tparam T Value type (can be modint for modular arithmetic).
 *
 * @note Multiplication includes an optimization to skip zero elements.
 * @note Power operation requires square matrices.
 */
template <typename T> class matrix {
  vector<vector<T>> v;

public:
  matrix(int n, int m, const T &v0 = {}) : v(n, vector(m, v0)) {}
  matrix(const vector<vector<T>> &v0) : v(v0) {}

  /**
   * @brief Returns the identity matrix of size `NxN`.
   *
   * @param n The dimension.
   * @param one Identity element along the diagonal.
   */
  static matrix identity(int n, T one = T{1}) {
    auto mat = matrix(n, n);
    for (int i = 0; i < n; i++)
      mat.v[i][i] = one;
    return mat;
  }

  /**
   * @brief Returns mutable reference of the element at `A(i, j)`.
   *
   * @param i First index.
   * @param j Second index.
   */
  T &at(int i, int j) { return v[i][j]; }

  /**
   * @brief Compute `A^P mod M`.
   *
   * @param p The power.
   */
  matrix pow(long long p) const {
    int n = v.size();
    auto res = matrix::identity(n);
    auto pw = matrix(*this);
    while (p) {
      if (p & 1)
        res = res * pw;
      pw = pw * pw;
      p >>= 1;
    }
    return res;
  }

#define OP(op)                                                                 \
  matrix &operator op## = (const matrix &other) {                              \
    int n = v.size(), m = v[0].size();                                         \
    for (int i = 0; i < n; i++)                                                \
      for (int j = 0; j < m; j++)                                              \
        v[i][j] op## = other.v[i][j];                                          \
    return *this;                                                              \
  }                                                                            \
  friend matrix operator op(const matrix &a, const matrix &b) {                \
    auto c = a;                                                                \
    c op## = b;                                                                \
    return c;                                                                  \
  }
  OP(+)
  OP(-)
#undef OP

  matrix &operator*=(const matrix &other) {
    int n = v.size(), c = v[0].size(), m = other.v[0].size();
    vector res(n, vector<T>(m));
    for (int i = 0; i < n; i++)
      for (int k = 0; k < c; k++) {
        if (!v[i][k])
          continue; // deceivingly important optimization (blame LeetCode)
        for (int j = 0; j < m; j++)
          res[i][j] += v[i][k] * other.v[k][j];
      }
    v = std::move(res);
    return *this;
  }

  friend matrix operator*(const matrix &a, const matrix &b) {
    auto c = a;
    c *= b;
    return c;
  }
};