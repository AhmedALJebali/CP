
const int MAXN = 100005;
const int LOG = 18;

int n, q;
vector<int> adj[MAXN];

int timer = 0;
int st[MAXN], en[MAXN], euler[2 * MAXN];
int depth[MAXN];
int up[MAXN][LOG];

int BLOCK_SIZE;
int node_freq[MAXN];
int current_ans = 0;
int ans[MAXN];

void dfs(int u, int p) {
    st[u] = ++timer;
    euler[timer] = u;
    up[u][0] = p;
    for (int i = 1; i < LOG; i++) {
        up[u][i] = up[up[u][i - 1]][i - 1];
    }
    for (int v : adj[u]) {
        if (v != p) {
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }
    en[u] = ++timer;
    euler[timer] = u;
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

struct Query {
    int l, r, id, lca;
    bool operator<(const Query& other) const {
        int b1 = l / BLOCK_SIZE;
        int b2 = other.l / BLOCK_SIZE;
        if (b1 != b2) return b1 < b2;
        return (b1 & 1) ? r < other.r : r > other.r;
    }
};
vector<Query> queries;

void toggle(int u) {
    if (node_freq[u] == 1) {
        node_freq[u] = 0;
    } else {
        node_freq[u] = 1;
    }
}
void solve() {
    cin >> n >> q;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    depth[1] = 0;
    dfs(1, 1);

    BLOCK_SIZE = max(1LL, (int)sqrt(2 * n));

    for (int i = 0; i < q; i++) {
        int u, v;
        cin >> u >> v;
        if (st[u] > st[v]) swap(u, v);
        int lca = get_lca(u, v);
        if (lca == u) {
            queries.push_back({st[u], st[v], i, -1});
        } else {
            queries.push_back({en[u], st[v], i, lca});
        }
    }
    sort(queries.begin(), queries.end());
    int L = 1, R = 0; 
    for (Query q : queries) {
        while (L > q.l) { L--; toggle(euler[L]); }
        while (R < q.r) { R++; toggle(euler[R]); }
        while (L < q.l) { toggle(euler[L]); L++; }
        while (R > q.r) { toggle(euler[R]); R--; }
        if (q.lca != -1) toggle(q.lca);
        ans[q.id] = current_ans;
        if (q.lca != -1) toggle(q.lca);
    }
    for (int i = 0; i < q; i++) {
        cout << ans[i] << "\n";
    }
}
