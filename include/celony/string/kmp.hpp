#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Compute the phi function from KMP.
 *
 * @param s The string.
 */
vector<int> kmp(const string &s) {
  int n = s.size();
  vector<int> pi(n);
  for (int i = 1; i < n; i++) {
    int j = pi[i - 1];
    for (; j > 0 && s[j] != s[i]; j = pi[j - 1]) {
    }
    if (s[j] == s[i])
      j++;
    pi[i] = j;
  }
  return pi;
}
