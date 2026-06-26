//  https://codeforces.com/gym/699485/problem/K
// 1. Coordinate Compression Struct
struct CoordCompressor {
    vector<int> coords;

    void add(int x) {
        coords.push_back(x);
    }

    void build() {
        sort(coords.begin(), coords.end());
        coords.erase(unique(coords.begin(), coords.end()), coords.end());
    }

    int get_id(int x) {
        return lower_bound(coords.begin(), coords.end(), x) - coords.begin();
    }

    int size() {
        return coords.size();
    }
};

// 2. Lazy Segment Tree Struct
struct LazySegTree {
    struct Node {
        int sum = 0;
        int lazy = 0;
        int len = 0; // Represents the real length of the interval (distance between coordinates)
        bool is_lazy = false;

        void apply(int val) {
            sum = (sum + (val % MOD) * len) % MOD;
            lazy = (lazy + val) % MOD;
            is_lazy = true;
        }
    };

    int n;
    vector<Node> tree;
    vector<int> coords; // Keep a copy of the coordinates to know interval lengths

    LazySegTree(const vector<int>& c) : coords(c) {
        n = 1;
        while (n < (int)coords.size()) n *= 2;
        tree.assign(2 * n, Node());
        build(0, 0, n);
    }

    Node merge(const Node& left, const Node& right) {
        Node res;
        res.sum = (left.sum + right.sum) % MOD;
        res.len = (left.len + right.len) % MOD;
        return res;
    }

    void build(int node, int lx, int rx) {
        if (rx - lx == 1) {
            if (lx < (int)coords.size() - 1) {
                // The length of the leaf is the difference between the current point and the next point
                tree[node].len = (coords[lx + 1] - coords[lx]) % MOD;
            }
            return;
        }
        int mid = (lx + rx) / 2;
        build(2 * node + 1, lx, mid);
        build(2 * node + 2, mid, rx);
        tree[node] = merge(tree[2 * node + 1], tree[2 * node + 2]);
    }

    void push_down(int node) {
        if (!tree[node].is_lazy) return;
        int val = tree[node].lazy;
        tree[2 * node + 1].apply(val);
        tree[2 * node + 2].apply(val);
        tree[node].lazy = 0;
        tree[node].is_lazy = false;
    }

    void update(int l, int r, int val, int node, int lx, int rx) {
        if (lx >= r || rx <= l) return;
        if (lx >= l && rx <= r) {
            tree[node].apply(val);
            return;
        }
        push_down(node);
        int mid = (lx + rx) / 2;
        update(l, r, val, 2 * node + 1, lx, mid);
        update(l, r, val, 2 * node + 2, mid, rx);
        tree[node] = merge(tree[2 * node + 1], tree[2 * node + 2]);
    }

    void update(int l, int r, int val) {
        update(l, r, val, 0, 0, n);
    }

    Node query(int l, int r, int node, int lx, int rx) {
        if (lx >= r || rx <= l) return Node();
        if (lx >= l && rx <= r) return tree[node];
        push_down(node);
        int mid = (lx + rx) / 2;
        Node left = query(l, r, 2 * node + 1, lx, mid);
        Node right = query(l, r, 2 * node + 2, mid, rx);
        return merge(left, right);
    }

    int query(int l, int r) {
        return query(l, r, 0, 0, n).sum;
    }
};
