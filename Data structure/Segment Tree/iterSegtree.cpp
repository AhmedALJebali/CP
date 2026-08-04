struct Node {
    int val;
    Node() { val = 0; }
    Node(int x) { val = x; }
    void change(int x) { val = x; }
};

struct segment_tree {
    int tree_size;
    vector<Node> segData;
    segment_tree(int n) {
        tree_size = 1;
        while (tree_size < n) tree_size *= 2;
        segData.assign(2 * tree_size, Node());
    }
    Node merge(const Node &left, const Node &right) {
        Node res;
        res.val = left.val + right.val;
        return res;
    }
    void init(const vector<int> &a) {
        for (int i = 0; i < (int)a.size(); i++) {
            segData[tree_size - 1 + i] = Node(a[i]);
        }
        for (int i = tree_size - 2; i >= 0; i--) {
            segData[i] = merge(segData[2 * i + 1], segData[2 * i + 2]);
        }
    }
    int get(int l, int r) {
        Node resL = Node();
        Node resR = Node();
        l += tree_size - 1;
        r += tree_size - 1;
        while (l < r) {
            if (l % 2 == 0) {
                resL = merge(resL, segData[l++]);
            }
            if (r % 2 == 0) {
                resR = merge(segData[--r], resR);
            }
            l = (l - 1) / 2;
            r = (r - 1) / 2;
        }
        return merge(resL, resR).val;
    }
    void update(int idx, int val) {
        idx += tree_size - 1;
        segData[idx].change(val);
        while (idx > 0) {
            idx = (idx - 1) / 2;
            segData[idx] = merge(segData[2 * idx + 1], segData[2 * idx + 2]);
        }
    }
};
