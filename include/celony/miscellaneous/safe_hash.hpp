#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Cryptographically-inspired hash function resistant to collision attacks.
 *
 * Standard unordered_map hash functions are vulnerable to adversarial input
 * that causes many collisions, degrading performance to \f$O(N)\f$ per operation.
 * This hasher uses a random seed and mixing functions to prevent such attacks.
 *
 * Time Complexity: \f$O(1)\f$ per hash
 * Space Complexity: \f$O(1)\f$
 *
 * @note Essential for contests where adversaries can craft anti-hash test cases.
 * @note Uses a random seed initialized at program startup.
 * @see https://codeforces.com/blog/entry/62393
 */
struct safe_hash {
  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM =
        chrono::steady_clock::now().time_since_epoch().count();
    x += FIXED_RANDOM + 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x;
  }
};
template <typename K, typename V>
using hash_map = unordered_map<K, V, safe_hash>;
template <typename V> using hash_set = unordered_set<V, safe_hash>;