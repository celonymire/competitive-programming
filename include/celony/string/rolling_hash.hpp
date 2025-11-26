#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Get a substring hash in \f$O(1)\f$ with \f$O(N)\f$ precomputation.
 *
 * This implementation uses one big modulo to avoid keeping two pairs of P and
 * M. Also, the hashes are computed in such a way it avoids needing to divide by
 * powers of P.
 */
class rolling_hash {
  static constexpr uint64_t M = (1ULL << 61) - 1, P = 257;
  inline static vector<uint64_t> p = vector<uint64_t>{1};
  vector<uint64_t> h;

public:
  rolling_hash(const string &s) : h(s.size() + 1) {
    int n = s.size();
    for (uint64_t i = p.size(), pp = p.back(); i <= n; i++)
      p.push_back(pp = __uint128_t(pp) * P % M);
    for (int i = n - 1; i >= 0; i--)
      h[i] = (s[i] + __uint128_t(h[i + 1]) * P % M) % M;
  }

  /**
   * @brief Get the hash of `s[l..r)`.
   *
   * @param l Left boundary.
   * @param r Right boundary.
   */
  uint64_t hash(int l, int r) const {
    return (h[l] - __uint128_t(h[r]) * p[r - l] % M + M) % M;
  }
};