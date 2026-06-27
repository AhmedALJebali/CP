struct Node {
    long long sum, lazy;
    int is_lazy;
    int lc, rc; // Indices for left and right children

    Node() { 
        sum = lazy = is_lazy = 0; 
        lc = rc = -1; 
    }

    Node(long long x) {
        sum = x;
        lazy = is_lazy = 0;
        lc = rc = -1;
    }

    void change(long long lx, long long rx, long long x) {
        long long len = (rx - lx);
        sum += (x * len);
        lazy += x;
        is_lazy = 1;
    }
};

struct DynamicSagara {
    long long min_val, max_val;
    vector<Node> segData;
    int root;
    DynamicSagara(long long min_val, long long max_val) {
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
        res.sum = left.sum + right.sum;
        return res;
    }
    void propagate(int node, long long lx, long long rx) {
        if (rx - lx == 1) return; 
        // Dynamically create children if they don't exist
        if (segData[node].lc == -1) segData[node].lc = create_node();
        if (segData[node].rc == -1) segData[node].rc = create_node();
        if (segData[node].is_lazy == 0) return;
        long long mid = lx + (rx - lx) / 2;
        long long x = segData[node].lazy;
        // Propagate to the dynamically created children
        segData[segData[node].lc].change(lx, mid, x);
        segData[segData[node].rc].change(mid, rx, x);
        segData[node].is_lazy = 0;
        segData[node].lazy = 0;
    }

    void set(long long idx, long long val, int node, long long lx, long long rx) {
        if (rx - lx == 1) {
            segData[node].sum = val; // Update directly to preserve lc/rc as -1
            return;
        }
        propagate(node, lx, rx);
        long long mid = lx + (rx - lx) / 2;
        
        if (idx < mid) {
            set(idx, val, segData[node].lc, lx, mid);
        } else {
            set(idx, val, segData[node].rc, mid, rx);
        }
        
        segData[node].sum = segData[segData[node].lc].sum + segData[segData[node].rc].sum;
    }

    void set(long long idx, long long val) { 
        set(idx, val, root, min_val, max_val); 
    }

    void update(long long l, long long r, long long x, int node, long long lx, long long rx) {
        if (lx >= l && rx <= r) {
            segData[node].change(lx, rx, x);
            return;
        }
        if (lx >= r || rx <= l) return;
        
        propagate(node, lx, rx);
        long long mid = lx + (rx - lx) / 2;
        
        update(l, r, x, segData[node].lc, lx, mid);
        update(l, r, x, segData[node].rc, mid, rx);
        
        segData[node].sum = segData[segData[node].lc].sum + segData[segData[node].rc].sum;
    }

    void update(long long l, long long r, long long x) { 
        update(l, r, x, root, min_val, max_val); 
    }

    Node get(long long l, long long r, int node, long long lx, long long rx) {
        if (node == -1 || lx >= r || rx <= l) return Node();
        if (lx >= l && rx <= r) {
            return segData[node];
        }
        propagate(node, lx, rx);
        long long mid = lx + (rx - lx) / 2;
        
        Node left = get(l, r, segData[node].lc, lx, mid);
        Node right = get(l, r, segData[node].rc, mid, rx);
        return merge(left, right);
    }
    Node get(long long l, long long r) { 
        return get(l, r, root, min_val, max_val); 
    }
};
