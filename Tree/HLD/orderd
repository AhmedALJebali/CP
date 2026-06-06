// -------------------- SEGMENT TREE NODE --------------------
struct Node {
    long long sum, lazy;
    bool is_lazy;
    
    Node() { 
        sum = lazy = 0; 
        is_lazy = false; 
    }
    
    Node(long long x) { 
        sum = x; 
        lazy = 0; 
        is_lazy = false; 
    }
    
    void change(int lx, int rx, long long x) {
        long long len = (rx - lx);
        sum += (x * len);
        lazy += x;
        is_lazy = true;
    }
};

// -------------------- LAZY SEGMENT TREE --------------------
struct SegmentTree {
    int sz;
    vector<Node> segData;
    
    // Empty constructor, initialized later by HLD
    SegmentTree() {}

    void init(int n) {
        sz = 1;
        while (sz < n) sz *= 2;
        segData.assign(2 * sz, Node());
    }

    // IMPORTANT: For non-commutative operations, order matters here! (left + right)
    Node merge(Node left, Node right) {
        Node res;
        res.sum = left.sum + right.sum;
        return res;
    }

    void propagate(int node, int lx, int rx) {
        if (!segData[node].is_lazy || rx - lx == 1) return;
        int mid = (lx + rx) / 2;
        long long x = segData[node].lazy;
        
        segData[2 * node + 1].change(lx, mid, x);
        segData[2 * node + 2].change(mid, rx, x);
        
        segData[node].is_lazy = false;
        segData[node].lazy = 0;
    }

    void set(int idx, int val, int node, int lx, int rx) {
        if (rx - lx == 1) {
            segData[node] = Node(val);
            return;
        }
        int mid = (lx + rx) / 2;
        if (idx < mid) set(idx, val, 2 * node + 1, lx, mid);
        else set(idx, val, 2 * node + 2, mid, rx);
        
        segData[node] = merge(segData[2 * node + 1], segData[2 * node + 2]);
    }
    void set(int idx, int val) { set(idx, val, 0, 0, sz); }

    void update(int l, int r, int x, int node, int lx, int rx) {
        propagate(node, lx, rx);
        if (lx >= l && rx <= r) {
            segData[node].change(lx, rx, x);
            return;
        }
        if (lx >= r || rx <= l) return;
        int mid = (lx + rx) / 2;
        update(l, r, x, 2 * node + 1, lx, mid);
        update(l, r, x, 2 * node + 2, mid, rx);
        segData[node] = merge(segData[2 * node + 1], segData[2 * node + 2]);
    }
    void update(int l, int r, int x) { update(l, r, x, 0, 0, sz); }

    Node get(int l, int r, int node, int lx, int rx) {
        propagate(node, lx, rx);
        if (lx >= l && rx <= r) return segData[node];
        if (lx >= r || rx <= l) return Node();
        
        int mid = (lx + rx) / 2;
        Node left = get(l, r, 2 * node + 1, lx, mid);
        Node right = get(l, r, 2 * node + 2, mid, rx);
        return merge(left, right);
    }
    Node get(int l, int r) { return get(l, r, 0, 0, sz); }
};

// -------------------- HEAVY-LIGHT DECOMPOSITION --------------------
struct HLD {
    int N, cur = 0;
    vector<int> par, depth, head, pos, sz;
    SegmentTree down, up; // down: normal order, up: reversed order

    HLD(vector<vector<int>>& adj, vector<int>& val)
        : N(adj.size()), par(N), depth(N), head(N), pos(N), sz(N) 
    {
        dfs_sz(0, 0, adj);
        dfs_hld(0, 0, adj);
        down.init(N);
        up.init(N);
        for (int i = 0; i < N; ++i) update_point(i, val[i]);
    }

    void dfs_sz(int u, int p, vector<vector<int>>& a) {
        sz[u] = 1; 
        par[u] = p; 
        depth[u] = (u == p ? 0 : depth[p] + 1);
        
        for (size_t i = 0; i < a[u].size(); ++i) {
            int v = a[u][i]; 
            if (v == p) continue;
            dfs_sz(v, u, a); 
            sz[u] += sz[v];
            if (a[u].size() && (a[u][0] == p || sz[v] > sz[a[u][0]])) {
                swap(a[u][i], a[u][0]);
            }
        }
    }

    void dfs_hld(int u, int h, vector<vector<int>>& a) {
        head[u] = h; 
        pos[u] = cur++;
        for (int v : a[u]) {
            if (v != par[u]) {
                dfs_hld(v, (v == a[u][0] ? h : v), a);
            }
        }
    }

    void update_point(int u, int c) {
        down.set(pos[u], c);
        up.set(N - 1 - pos[u], c);
    }

    void update_range_pos(int lpos, int rpos, int c) {
        down.update(lpos, rpos + 1, c);
        int L = N - 1 - rpos;
        int R = N - 1 - lpos;
        up.update(L, R + 1, c);
    }

    void update_path(int u, int v, int c) {
        int x = u, y = v;
        while (head[x] != head[y]) {
            if (depth[head[x]] > depth[head[y]]) {
                update_range_pos(pos[head[x]], pos[x], c);
                x = par[head[x]];
            } else {
                update_range_pos(pos[head[y]], pos[y], c);
                y = par[head[y]];
            }
        }
        int l = min(pos[x], pos[y]);
        int r = max(pos[x], pos[y]);
        update_range_pos(l, r, c);
    }

    Node query_path(int u, int v) {
        Node left, right; 
        int x = u, y = v;
        
        while (head[x] != head[y]) {
            if (depth[head[x]] > depth[head[y]]) {
                // Climbing up from u (reads exactly from deeper to higher using the 'up' tree)
                int l = N - 1 - pos[x];
                int r = N - 1 - pos[head[x]];
                left = up.merge(left, up.get(l, r + 1));
                x = par[head[x]];
            } else {
                // Climbing up from v (reads exactly from higher to deeper using the 'down' tree)
                int l = pos[head[y]];
                int r = pos[y];
                right = down.merge(down.get(l, r + 1), right);
                y = par[head[y]];
            }
        }
        
        // Process the final segment containing the LCA
        if (depth[x] > depth[y]) {
            int l = N - 1 - pos[x];
            int r = N - 1 - pos[y];
            left = up.merge(left, up.get(l, r + 1));
        } else {
            int l = pos[x];
            int r = pos[y];
            right = down.merge(down.get(l, r + 1), right);
        }
        
        // Final chronological merge: Path from 'u' UP to LCA, then DOWN to 'v'
        return down.merge(left, right);
    }
};
