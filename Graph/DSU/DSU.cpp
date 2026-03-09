struct DSU {
  vector<int> p, sz;
  int comp, mx;

  DSU(int n) {
    comp = n;
    mx = 1;
    p.resize(n + 1);
    sz.resize(n + 1);
    for (int i = 0; i <= n; i++) {
      p[i] = i;
      sz[i] = 1;
    }
  }

  int f(int node) { return p[node] == node ? node : p[node] = f(p[node]); }

  bool mrg(int u, int v) {
    u = f(u);
    v = f(v);

    if (u == v) {
      return false;
    }
    if (sz[v] > sz[u])
      swap(u, v);
    p[v] = u;
    sz[u] += sz[v];
    mx = max(mx, sz[u]);
    return true;
  }

  int gz(int node) { return sz[f(node)]; }
  bool isconnected(int u, int v) { return f(u) == f(v); }
};