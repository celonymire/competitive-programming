#include <bits/stdc++.h>
using namespace std;

/**
 * @brief The extended GCD algorithm that finds a solution for `ax+by=gcd(a,b)`.
 *
 * @param a First number.
 * @param b Second number.
 * @param x Reference to store `x`.
 * @param y Reference to store `y`.
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