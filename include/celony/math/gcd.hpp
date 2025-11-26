#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Extended Euclidean Algorithm for finding Bézout coefficients.
 *
 * Computes integers x and y such that ax + by = gcd(a, b). This is useful
 * for finding modular inverses and solving linear Diophantine equations.
 *
 * Time Complexity: O(log(min(a, b)))
 * Space Complexity: O(1)
 *
 * @param a First number.
 * @param b Second number.
 * @param x Reference to store the coefficient for a.
 * @param y Reference to store the coefficient for b.
 * @return gcd(a, b)
 *
 * @note To find modular inverse of a mod b (when gcd(a,b)=1),
 *       x will be the inverse after calling this function.
 */
template <typename T> T gcd(T a, T b, T &x, T &y) {
  x = 1, y = 0;
  T x1 = 0, y1 = 1, a1 = a, b1 = b;
  while (b1) {
    int q = a1 / b1;
    tie(x, x1) = make_tuple(x1, x - q * x1);
    tie(y, y1) = make_tuple(y1, y - q * y1);
    tie(a1, b1) = make_tuple(b1, a1 - q * b1);
  }
  return a1;
}