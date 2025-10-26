#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Node class for the Aho-Corasick Automaton.
 *
 * @tparam Begin Lowest character in range.
 * @tparam End Highest character in range.
 */
template <int Begin, int End> struct aho_corasick_node_t {
  int next[End - Begin + 1]{}; // Next node index from a given character.
  int link = 0;                // Suffix link.
  int exit = 0;                // Next match link.
  vector<int> matches;         // Index of matches in provided patterns.
};

/**
 * @brief Computes the automaton to efficiently match the given patterns.
 *
 * Dedicated template parameters denoting the ASCII range of characters are
 * provided to create C-style arrays for adjacent list (blame CodeForces).
 *
 * @tparam Begin Lowest character in range.
 * @tparam End Highest character in range.
 */
template <int Begin, int End>
vector<aho_corasick_node_t<Begin, End>>
aho_corasick_automaton(const vector<string> &p) {
  int m = p.size();
  vector<aho_corasick_node_t<Begin, End>> g(1);
  for (int i = 0; i < m; i++) {
    int u = 0;
    for (char c : p[i]) {
      if (!g[u].next[c - Begin]) {
        g[u].next[c - Begin] = g.size();
        g.push_back({});
      }
      u = g[u].next[c - Begin];
    }
    g[u].matches.push_back(i);
  }
  queue<int> q{{0}};
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int a = 0; a <= End - Begin; a++) {
      if (int v = g[u].next[a]) {
        g[v].link = u ? g[g[u].link].next[a] : 0;
        g[v].exit =
            g[g[v].link].matches.empty() ? g[g[v].link].exit : g[v].link;
        q.push(v);
      } else
        g[u].next[a] = g[g[u].link].next[a];
    }
  }
  return g;
}