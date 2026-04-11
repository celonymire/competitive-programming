// https://cses.fi/problemset/model/1676/

#include <algorithm>
#include <iostream>
#include <numeric>

using namespace std;
const int N = 100001;

int sz[N];
int link[N];

int find(int x) {
  if (link[x] == x) {
    return x;
  }
  return link[x] = find(link[x]);
}

bool same(int x, int y) { return find(x) == find(y); }

void unite(int x, int y) {
  x = find(x);
  y = find(y);
  if (sz[x] < sz[y]) {
    swap(x, y);
  }
  sz[x] += sz[y];
  link[y] = x;
}

int main() {
  int n, m;
  cin >> n >> m;

  fill(sz, sz + n + 1, 1);
  iota(link, link + n + 1, 0);

  int n_components = n;
  int largest_size = 1;

  for (int i = 0; i < m; ++i) {
    int a, b;
    cin >> a >> b;
    if (!same(a, b)) {
      unite(a, b);
      --n_components;
      largest_size = max(sz[find(a)], largest_size);
    }
    cout << boolalpha << (find(a) == find(b)) << '\n'; // my own test
    cout << n_components << ' ' << largest_size << '\n';
  }
}