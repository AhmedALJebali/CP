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
const int BASE1 = 313, BASE2 = 317, MOD1 = 1e9 + 7, MOD2 = 1e9 + 9, MAXN = 500005;
int pw1[MAXN], pw2[MAXN];
void precompute_powers() {
    pw1[0] = 1; pw2[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        pw1[i] = (pw1[i - 1] * BASE1) % MOD1;
        pw2[i] = (pw2[i - 1] * BASE2) % MOD2;
    }
}
struct Node {
    int fwd1, fwd2;
    int rev1, rev2;
    int len;
    Node() : fwd1(0), fwd2(0), rev1(0), rev2(0), len(0) {}
    Node(char c) {
        fwd1 = rev1 = c;
        fwd2 = rev2 = c;
        len = 1;
    }
};
Node combine(const Node& a, const Node& b) {
    if (a.len == 0) return b;
    if (b.len == 0) return a;
    Node res;
    res.len = a.len + b.len;
    res.fwd1 = (a.fwd1 * pw1[b.len] % MOD1 + b.fwd1) % MOD1;
    res.rev1 = (b.rev1 * pw1[a.len] % MOD1 + a.rev1) % MOD1;
    res.fwd2 = (a.fwd2 * pw2[b.len] % MOD2 + b.fwd2) % MOD2;
    res.rev2 = (b.rev2 * pw2[a.len] % MOD2 + a.rev2) % MOD2;
    return res;
}
Node reverse_node(Node a) {
    swap(a.fwd1, a.rev1);
    swap(a.fwd2, a.rev2);
    return a;
}
struct HLD_String {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, sz;
    vector<Node> tree;
    string s;
    HLD_String(int n) {
        this->n = n;
        adj.assign(n + 1, vector<int>());
        parent.assign(n + 1, 0);
        depth.assign(n + 1, 0);
        heavy.assign(n + 1, -1);
        head.assign(n + 1, 0);
        pos.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        tree.assign(4 * n + 1, Node());
        timer = 1;
    }
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int dfs_sz(int v, int p) {
        sz[v] = 1;
        parent[v] = p;
        int max_sub = 0;
        for (int c : adj[v]) {
            if (c != p) {
                depth[c] = depth[v] + 1;
                int sub_sz = dfs_sz(c, v);
                sz[v] += sub_sz;
                if (sub_sz > max_sub) {
                    max_sub = sub_sz;
                    heavy[v] = c;
                }
            }
        }
        return sz[v];
    }
    void dfs_hld(int v, int p, int h) {
        head[v] = h;
        pos[v] = timer++;
        update(1, 1, n, pos[v], s[v - 1]);
        if (heavy[v] != -1) {
            dfs_hld(heavy[v], v, h);
        }
        for (int c : adj[v]) {
            if (c != p && c != heavy[v]) {
                dfs_hld(c, v, c);
            }
        }
    }
    void init(string _s, int root = 1) {
        s = _s;
        dfs_sz(root, 0);
        dfs_hld(root, 0, root);
    }
    void update(int node, int start, int end, int idx, char val) {
        if (start == end) {
            tree[node] = Node(val);
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) update(2 * node, start, mid, idx, val);
        else update(2 * node + 1, mid + 1, end, idx, val);
        tree[node] = combine(tree[2 * node], tree[2 * node + 1]);
    }
    Node query_tree(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return Node();
        if (l <= start && end <= r) return tree[node];
        int mid = (start + end) / 2;
        Node left = query_tree(2 * node, start, mid, l, r);
        Node right = query_tree(2 * node + 1, mid + 1, end, l, r);
        return combine(left, right);
    }
    Node query_path(int u, int v) {
        Node ans_u = Node();
        Node ans_v = Node();
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                Node chunk = query_tree(1, 1, n, pos[head[u]], pos[u]);
                ans_u = combine(ans_u, reverse_node(chunk));
                u = parent[head[u]];
            } else {
                Node chunk = query_tree(1, 1, n, pos[head[v]], pos[v]);
                ans_v = combine(chunk, ans_v);
                v = parent[head[v]];
            }
        }
        if (depth[u] > depth[v]) {
            Node chunk = query_tree(1, 1, n, pos[v], pos[u]);
            ans_u = combine(ans_u, reverse_node(chunk));
        } else {
            Node chunk = query_tree(1, 1, n, pos[u], pos[v]);
            ans_v = combine(chunk, ans_v);
        }
        return combine(ans_u, ans_v);
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    HLD_String hld(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        hld.add_edge(u, v);
    }
    string s;
    cin >> s;
    hld.init(s);
    set<pair<int, int>> st;
    for (int i = 0; i < m; i++) {
        string t;
        cin >> t;
        int cur1 = 0, cur2 = 0;
        for (int k = 0; k < t.size(); k++) {
            cur1 = (cur1 * BASE1 + t[k]) % MOD1;
            cur2 = (cur2 * BASE2 + t[k]) % MOD2;
        }
        st.insert({cur1, cur2});
    }
    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;
        Node p = hld.query_path(u, v);
        if (st.count({p.fwd1, p.fwd2})) {
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
