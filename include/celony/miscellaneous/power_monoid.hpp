template <int MOD = (int)1e9 + 7, typename T = long long> struct power_monoid {
  constexpr T operator()(const T &a, const T &b) const noexcept {
    return 1ll * a * b % MOD;
  }
  friend constexpr T identity_element(power_monoid<MOD, T>) noexcept {
    return 1;
  }
};