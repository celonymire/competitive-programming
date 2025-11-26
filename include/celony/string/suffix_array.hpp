#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Compute the suffix array of a given string in \f$O(N \log N)\f$.
 *
 * Even though this is not the optimal \f$O(N)\f$ time complexity, the benefit is
 * the algorithm works with cyclic shifts. For example, `SA[0]` is the smallest
 * cyclic shift.
 *
 * @param s The string
 * @param k Size of the alphabet.
 */
vector<int> suffix_array(string &s, int k = 256) {
  s.push_back('$');
  int n = s.size();
  vector<int> p(n), c(n), cnt(max(k, n), 0);
  for (int i = 0; i < n; i++)
    cnt[s[i]]++;
  for (int i = 1; i < k; i++)
    cnt[i] += cnt[i - 1];
  for (int i = 0; i < n; i++)
    p[--cnt[s[i]]] = i;
  c[p[0]] = 0;
  int classes = 1;
  for (int i = 1; i < n; i++) {
    if (s[p[i]] != s[p[i - 1]])
      classes++;
    c[p[i]] = classes - 1;
  }
  vector<int> pn(n), cn(n);
  for (int h = 0; (1 << h) < n; ++h) {
    for (int i = 0; i < n; i++) {
      pn[i] = p[i] - (1 << h);
      if (pn[i] < 0)
        pn[i] += n;
    }
    fill(cnt.begin(), cnt.begin() + classes, 0);
    for (int i = 0; i < n; i++)
      cnt[c[pn[i]]]++;
    for (int i = 1; i < classes; i++)
      cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--)
      p[--cnt[c[pn[i]]]] = pn[i];
    cn[p[0]] = 0;
    classes = 1;
    for (int i = 1; i < n; i++) {
      pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
      pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
      if (cur != prev)
        ++classes;
      cn[p[i]] = classes - 1;
    }
    c.swap(cn);
  }
  s.pop_back();
  p.erase(p.begin());
  return p;
}