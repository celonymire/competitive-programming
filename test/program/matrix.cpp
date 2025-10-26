#include <bits/stdc++.h>
using namespace std;

#include <celony/math/matrix.hpp>
#include <celony/math/modint.hpp>

class Solution {
public:
  int zigZagArrays(int n, int l, int r) {
    int m = r - l + 1;
    if (n == 1)
      return m;
    const int M = 1e9 + 7;
    matrix<modint<M>> t(2 * m, 2 * m, 0);
    for (int i = 0; i < m; i++) {
      for (int j = i + 1; j < m; j++)
        t.at(i, j + m) = 1;
      for (int j = 0; j < i; j++)
        t.at(i + m, j) = 1;
    }
    matrix<modint<M>> a(2 * m, 1, 1);
    auto dp = t.pow(n - 1) * a;
    modint<M> ans;
    for (int i = 0; i < 2 * m; i++)
      ans += dp.at(i, 0);
    return ans.value();
  }
};

int main() {
  int n, l, r;
  cin >> n >> l >> r;
  cout << Solution().zigZagArrays(n, l, r) << '\n';
}