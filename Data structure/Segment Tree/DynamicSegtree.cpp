const int MOD=1e9+7;
struct Node {
    int sum, lazy;
    int is_lazy;
    int lc, rc;
    Node() {
        sum = lazy = is_lazy = 0;
        lc = rc = -1;
    }
    Node(int x) {
        sum = x;
        lazy = is_lazy = 0;
        lc = rc = -1;
    }
    void change(int lx, int rx, int x) {
        int len = (rx - lx) % MOD;
        sum = (sum + (x % MOD) * len) % MOD;
        lazy = (lazy + x) % MOD;
        is_lazy = 1;
    }
};

struct DynamicSagara {
    int min_val, max_val;
    vector<Node> segData;
    int root;

    DynamicSagara(int min_val, int max_val) {
        this->min_val = min_val;
        this->max_val = max_val;
        root = create_node();
    }

    int create_node() {
        segData.push_back(Node());
        return segData.size() - 1;
    }

    Node merge(const Node& left, const Node& right) {
        Node res;
        res.sum = (left.sum + right.sum)%MOD;
        return res;
    }

    void propagate(int node, int lx, int rx) {
        if (rx - lx == 1) return;

        if (segData[node].lc == -1) segData[node].lc = create_node();
        if (segData[node].rc == -1) segData[node].rc = create_node();

        if (segData[node].is_lazy == 0) return;

        int mid = lx + (rx - lx) / 2;
        int x = segData[node].lazy;

        segData[segData[node].lc].change(lx, mid, x);
        segData[segData[node].rc].change(mid, rx, x);

        segData[node].is_lazy = 0;
        segData[node].lazy = 0;
    }

    void set(int idx, int val, int node, int lx, int rx) {
        if (rx - lx == 1) {
            segData[node].sum = val;
            segData[node].lazy = 0;
            segData[node].is_lazy = 0;
            return;
        }
        propagate(node, lx, rx);
        int mid = lx + (rx - lx) / 2;

        if (idx < mid) {
            set(idx, val, segData[node].lc, lx, mid);
        } else {
            set(idx, val, segData[node].rc, mid, rx);
        }

        segData[node].sum = segData[segData[node].lc].sum + segData[segData[node].rc].sum;
    }

    void set(int idx, int val) {
        set(idx, val, root, min_val, max_val);
    }

    void update(int l, int r, int x, int node, int lx, int rx) {
        if (lx >= r || rx <= l) return;
        if (lx >= l && rx <= r) {
            segData[node].change(lx, rx, x);
            return;
        }

        propagate(node, lx, rx);
        int mid = lx + (rx - lx) / 2;

        update(l, r, x, segData[node].lc, lx, mid);
        update(l, r, x, segData[node].rc, mid, rx);

        segData[node].sum = segData[segData[node].lc].sum + segData[segData[node].rc].sum;
    }

    void update(int l, int r, int x) {
        update(l, r, x, root, min_val, max_val);
    }

    Node get(int l, int r, int node, int lx, int rx) {
        if (node == -1 || lx >= r || rx <= l) return Node();
        if (lx >= l && rx <= r) {
            return segData[node];
        }
        propagate(node, lx, rx);
        int mid = lx + (rx - lx) / 2;

        Node left = get(l, r, segData[node].lc, lx, mid);
        Node right = get(l, r, segData[node].rc, mid, rx);
        return merge(left, right);
    }

    Node get(int l, int r) {
        return get(l, r, root, min_val, max_val);
    }
};

