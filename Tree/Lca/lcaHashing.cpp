#include<bits/stdc++.h>
using namespace std;
#define int long long
#define ld long double
#define all(v) v.begin(), v.end()
#define FAST                    \
ios_base::sync_with_stdio(0); \
cin.tie(nullptr);             \
cout.tie(nullptr);
#define fileIO                      \
freopen("input.txt", "r", stdin); \
freopen("out.txt", "w", stdout)

// --- Double Hashing Constants ---
const int BASE1 = 313;
const int BASE2 = 317;
const int MOD1 = 1e9 + 7;
const int MOD2 = 1e9 + 9;
const int MAXN = 500005;
const int LOG = 20;

int pw1[MAXN], pw2[MAXN];
int inv1[MAXN], inv2[MAXN];

// Modular exponentiation
int power(int base, int exp, int mod) {
    int res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

// Modular inverse
int modInverse(int n, int mod) {
    return power(n, mod - 2, mod);
}

// Precompute powers and their modular inverses
void precompute_powers() {
    pw1[0] = 1; pw2[0] = 1;
    inv1[0] = 1; inv2[0] = 1;

    int invB1 = modInverse(BASE1, MOD1);
    int invB2 = modInverse(BASE2, MOD2);

    for (int i = 1; i < MAXN; i++) {
        pw1[i] = (pw1[i - 1] * BASE1) % MOD1;
        pw2[i] = (pw2[i - 1] * BASE2) % MOD2;

        inv1[i] = (inv1[i - 1] * invB1) % MOD1;
        inv2[i] = (inv2[i - 1] * invB2) % MOD2;
    }
}

struct TreeHashLCA {
    int n;
    vector<vector<int>> adj;
    vector<int> depth;
    vector<vector<int>> up;
    string s;
    vector<int> H1, H2, R1, R2;
    TreeHashLCA(int n, string _s) {
        this->n = n;
        s = " " + _s; // 1-based indexing for the string
        adj.assign(n + 1, vector<int>());
        depth.assign(n + 1, 0);
        up.assign(n + 1, vector<int>(LOG, 0));
        H1.assign(n + 1, 0); H2.assign(n + 1, 0);
        R1.assign(n + 1, 0); R2.assign(n + 1, 0);
    }
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void dfs(int u, int p, int d) {
        up[u][0] = p;
        for (int i = 1; i < LOG; i++) {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }
        depth[u] = d;
        H1[u] = (H1[p] * BASE1 + s[u]) % MOD1;
        H2[u] = (H2[p] * BASE2 + s[u]) % MOD2;
        R1[u] = (R1[p] + s[u] * pw1[d]) % MOD1;
        R2[u] = (R2[p] + s[u] * pw2[d]) % MOD2;
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, d + 1);
            }
        }
    }
    void init(int root = 1) {
        depth[0] = -1; // Dummy parent depth
        dfs(root, 0, 0);
    }
    int get_lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int i = 0; i < LOG; i++) {
            if ((diff >> i) & 1) u = up[u][i];
        }
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }
    pair<int, int> query_path(int u, int v) {
        int lca = get_lca(u, v);
        int p = up[lca][0]; // Parent of LCA
        int hash1_up = (R1[u] - R1[p] + MOD1) % MOD1 * inv1[depth[lca]] % MOD1;
        int hash2_up = (R2[u] - R2[p] + MOD2) % MOD2 * inv2[depth[lca]] % MOD2;
        int len_down = depth[v] - depth[lca];
        int hash1_down = 0, hash2_down = 0;
        if (len_down > 0) {
            hash1_down = (H1[v] - H1[lca] * pw1[len_down] % MOD1 + MOD1) % MOD1;
            hash2_down = (H2[v] - H2[lca] * pw2[len_down] % MOD2 + MOD2) % MOD2;
        }
        int final_h1 = (hash1_up * pw1[len_down] % MOD1 + hash1_down) % MOD1;
        int final_h2 = (hash2_up * pw2[len_down] % MOD2 + hash2_down) % MOD2;
        return {final_h1, final_h2};
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<pair<int,int>> edges(n - 1);
    for (int i = 0; i < n - 1; i++) {
        cin >> edges[i].first >> edges[i].second;
    }
    string s;
    cin >> s;
    TreeHashLCA tree(n, s);
    for (auto e : edges) {
        tree.add_edge(e.first, e.second);
    }
    tree.init();
    set<pair<int, int>> st;
    for (int i = 0; i < m; i++) {
        string t;
        cin >> t;
        int cur1 = 0, cur2 = 0;
        for (char c : t) {
            cur1 = (cur1 * BASE1 + c) % MOD1;
            cur2 = (cur2 * BASE2 + c) % MOD2;
        }
        st.insert({cur1, cur2});
    }
    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;
        pair<int, int> path_hash = tree.query_path(u, v);
        if (st.count(path_hash)) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
}
signed main() {
  FAST;
#ifndef ONLINE_JUDGE
  fileIO;
#endif
  int t = 1;
  cin >> t;
  precompute_powers();
  for (int i = 1; i <= t; i++) {
    solve();
  }
  return 0;
};
