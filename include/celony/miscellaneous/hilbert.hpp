#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Computes Hilbert curve ordering for 2D points/queries.
 *
 * Maps 2D coordinates to 1D values along a space-filling Hilbert curve.
 * This ordering has excellent locality properties, making it ideal for
 * reordering queries in Mo's algorithm to minimize pointer movements.
 *
 * Time Complexity: O(Q log max(coordinates))
 * Space Complexity: O(Q)
 *
 * @param q List of 2D points/queries as (x, y) pairs.
 * @return Vector of 64-bit integers representing Hilbert curve positions.
 *         Sorting queries by these values optimizes Mo's algorithm.
 *
 * @note This is the recommended heuristic for Mo's algorithm.
 * @see https://codeforces.com/blog/entry/61203
 */
vector<uint64_t> hilbert(const vector<pair<int, int>> &q) {
  int n = q.size();
  vector<uint64_t> ans(n);
  for (int i = 0; i < n; i++) {
    auto [x, y] = q[i];
    const uint64_t l = __lg(max(x, y) * 2 + 1) | 1;
    const uint64_t mx = (1ull << l) - 1;
    uint64_t ord = 0;
    for (uint64_t s = 1ull << (l - 1); s; s >>= 1) {
      bool rx = x & s, ry = y & s;
      ord = (ord << 2) | (rx ? ry ? 2 : 1 : ry ? 3 : 0);
      if (!rx) {
        if (ry)
          x ^= mx, y ^= mx;
        swap(x, y);
      }
    }
    ans[i] = ord;
  }
  return ans;
}
