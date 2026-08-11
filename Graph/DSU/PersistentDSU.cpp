struct PersistentDSU {
    vector<int> p, sz, time;
    PersistentDSU(int n) {
        p.resize(n + 1);
        sz.resize(n + 1, 1);
        time.resize(n + 1, 1e9);
        for (int i = 0; i <= n; i++) {
            p[i] = i;
        }
    }
    int f(int node) {
        while (node != p[node]) {
            node = p[node];
        }
        return node;
    }
    // Merge u and v at day 't'
    bool mrg(int u, int v, int t) {
        u = f(u);
        v = f(v);
        if (u == v) {
            return false;
        }
        if (sz[v] > sz[u]) {
            swap(u, v);
        }
        p[v] = u;
        sz[u] += sz[v];
        time[v] = t;
        return true;
    }
    int f_time(int node, int t) {
        while (node != p[node] && time[node] <= t) {
            node = p[node];
        }
        return node;
    }
    bool isconnected(int u, int v, int t) {
        return f_time(u, t) == f_time(v, t);
    }
};
