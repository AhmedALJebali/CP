struct RollbackDSU {
    vector<int> p, sz;
    int comp, mx;
    struct Change {
        int u, v;
        int sz_u;
        int prev_comp;
        int prev_mx;
    };
    vector<Change> history;
    vector<int> checkpoints;

    RollbackDSU(int n) {
        comp = n;
        mx = 1;
        p.resize(n + 1);
        sz.resize(n + 1);
        for (int i = 0; i <= n; i++) {
            p[i] = i;
            sz[i] = 1;
        }
    }
    int f(int node) {
        while (node != p[node]) {
            node = p[node];
        }
        return node;
    }
    bool mrg(int u, int v) {
        u = f(u);
        v = f(v);
        if (u == v) {
            return false;
        }
        if (sz[v] > sz[u]) {
            swap(u, v);
        }
        history.push_back({u, v, sz[u], comp, mx});
        p[v] = u;
        sz[u] += sz[v];
        mx = max(mx, sz[u]);
        comp--;
        return true;
    }
    void persist() {
        checkpoints.push_back(history.size());
    }
    void rollback() {
        if (checkpoints.empty()) return;
        int target_size = checkpoints.back();
        checkpoints.pop_back();
        while (history.size() > target_size) {
            Change c = history.back();
            history.pop_back();
            p[c.v] = c.v;
            sz[c.u] = c.sz_u;
            comp = c.prev_comp;
            mx = c.prev_mx;
        }
    }

    int gz(int node) { return sz[f(node)]; }
    bool isconnected(int u, int v) { return f(u) == f(v); }
};
