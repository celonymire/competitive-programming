#include <bits/stdc++.h>
using namespace std;

#include <celony/math/combinatorics.hpp>

int main() {
  cin.tie(0)->sync_with_stdio(false);
  int n;
  cin >> n;
  combinatorics<(int)1e9 + 7> comb(1e6);
  for (int i = 0; i < n; i++) {
    int a, b;
    cin >> a >> b;
    cout << comb.ncr(a, b).value() << '\n';
  }
}