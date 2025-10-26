#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Computes the Hilbert Curve for the queries.
 *
 * @param q List of queries of `a[l..r]`.
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
