class WaveletTree {
private:
    struct Node {
        int lo, hi;
        int l, r;
        vector<int> b;
        vector<int> sum_l;

        Node() : lo(0), hi(0), l(-1), r(-1) {}
    };

    vector<Node> tree;
    int root;

    int build(int* from, int* to, int x, int y) {
        if (from >= to || x > y) return -1;

        int node_idx = tree.size();
        tree.emplace_back();
        tree[node_idx].lo = x;
        tree[node_idx].hi = y;

        if (x == y) return node_idx;

        int mid = x + (y - x) / 2;
        auto f = [mid](int c) { return c <= mid; };

        int n = to - from;
        tree[node_idx].b.assign(n + 1, 0);
        tree[node_idx].sum_l.assign(n + 1, 0);

        for (int i = 0; i < n; ++i) {
            bool goes_left = f(from[i]);
            tree[node_idx].b[i + 1] = tree[node_idx].b[i] + goes_left;
            tree[node_idx].sum_l[i + 1] = tree[node_idx].sum_l[i] + (goes_left ? from[i] : 0);
        }

        auto pivot = stable_partition(from, to, f);

        if (from != pivot) {
            tree[node_idx].l = build(from, pivot, x, mid);
        }
        if (pivot != to) {
            tree[node_idx].r = build(pivot, to, mid + 1, y);
        }

        return node_idx;
    }

    int kth(int node, int L, int R, int k) const {
        if (node == -1 || L > R) return -1;
        if (tree[node].lo == tree[node].hi) return tree[node].lo;

        int in_left = tree[node].b[R] - tree[node].b[L - 1];
        if (k <= in_left) {
            return kth(tree[node].l, tree[node].b[L - 1] + 1, tree[node].b[R], k);
        } else {
            return kth(tree[node].r, L - tree[node].b[L - 1], R - tree[node].b[R], k - in_left);
        }
    }

    int count_less(int node, int L, int R, int X) const {
        if (node == -1 || L > R || X <= tree[node].lo) return 0;
        if (tree[node].hi < X) return R - L + 1;
        if (tree[node].lo == tree[node].hi) return 0;

        int mid = tree[node].lo + (tree[node].hi - tree[node].lo) / 2;
        int in_left = tree[node].b[R] - tree[node].b[L - 1];

        if (X <= mid) {
            return count_less(tree[node].l, tree[node].b[L - 1] + 1, tree[node].b[R], X);
        } else {
            return in_left + count_less(tree[node].r, L - tree[node].b[L - 1], R - tree[node].b[R], X);
        }
    }

    int count_val(int node, int L, int R, int X) const {
        if (node == -1 || L > R || X < tree[node].lo || X > tree[node].hi) return 0;
        if (tree[node].lo == tree[node].hi) return R - L + 1;

        int mid = tree[node].lo + (tree[node].hi - tree[node].lo) / 2;
        if (X <= mid) {
            return count_val(tree[node].l, tree[node].b[L - 1] + 1, tree[node].b[R], X);
        } else {
            return count_val(tree[node].r, L - tree[node].b[L - 1], R - tree[node].b[R], X);
        }
    }

    int sum_k_smallest(int node, int L, int R, int k) const {
        if (node == -1 || L > R || k <= 0) return 0;
        if (tree[node].lo == tree[node].hi) return 1LL * tree[node].lo * k;

        int in_left = tree[node].b[R] - tree[node].b[L - 1];
        if (k <= in_left) {
            return sum_k_smallest(tree[node].l, tree[node].b[L - 1] + 1, tree[node].b[R], k);
        } else {
            int left_sum = tree[node].sum_l[R] - tree[node].sum_l[L - 1];
            return left_sum + sum_k_smallest(tree[node].r, L - tree[node].b[L - 1], R - tree[node].b[R], k - in_left);
        }
    }

    int majority(int node, int L, int R, int threshold) const {
        if (node == -1 || L > R) return -1;
        if (tree[node].lo == tree[node].hi) {
            return (R - L + 1 >= threshold) ? tree[node].lo : -1;
        }
        int in_left = tree[node].b[R] - tree[node].b[L - 1];
        int in_right = (R - L + 1) - in_left;
        if (in_left >= threshold) {
            return majority(tree[node].l, tree[node].b[L - 1] + 1, tree[node].b[R], threshold);
        }
        if (in_right >= threshold) {
            return majority(tree[node].r, L - tree[node].b[L - 1], R - tree[node].b[R], threshold);
        }
        return -1;
    }

public:
    WaveletTree() : root(-1) {}
    WaveletTree(const vector<int>& arr) {
        if (arr.empty()) {
            root = -1;
            return;
        }
        int min_val = *min_element(arr.begin(), arr.end());
        int max_val = *max_element(arr.begin(), arr.end());
        vector<int> arr_copy = arr;
        root = build(arr_copy.data(), arr_copy.data() + arr_copy.size(), min_val, max_val);
    }

    // --- NOTE: All queries assume 1-based indexing for L and R ---
    int get_kth_smallest(int L, int R, int k) const { return kth(root, L, R, k); }
    int get_kth_largest(int L, int R, int k) const { return kth(root, L, R, (R - L + 1) - k + 1); }
    int get_median(int L, int R) const { return kth(root, L, R, (R - L + 2) / 2); }
    int count_occurrences(int L, int R, int X) const { return count_val(root, L, R, X); }
    bool exists(int L, int R, int X) const { return count_occurrences(L, R, X) > 0; }
    int count_strictly_less(int L, int R, int X) const { return count_less(root, L, R, X); }
    int count_strictly_greater(int L, int R, int X) const { return (R - L + 1) - count_less(root, L, R, X + 1); }
    int count_between(int L, int R, int X, int Y) const { return count_less(root, L, R, Y + 1) - count_less(root, L, R, X); }
    int get_predecessor(int L, int R, int X) const {
        int count = count_strictly_less(L, R, X + 1);
        return (count == 0) ? -1 : get_kth_smallest(L, R, count);
    }
    int get_successor(int L, int R, int X) const {
        int count = count_strictly_less(L, R, X);
        return (count == R - L + 1) ? -1 : get_kth_smallest(L, R, count + 1);
    }
    int sum_k_smallest(int L, int R, int k) const { return sum_k_smallest(root, L, R, k); }
    int sum_k_largest(int L, int R, int k) const {
        int total = R - L + 1;
        return sum_k_smallest(root, L, R, total) - sum_k_smallest(root, L, R, total - k);
    }

    int sum_between_values(int L, int R, int X, int Y) const {
        return sum_k_smallest(root, L, R, count_strictly_less(L, R, Y + 1)) -
               sum_k_smallest(root, L, R, count_strictly_less(L, R, X));
    }

    int get_majority(int L, int R) const { return majority(root, L, R, (R - L + 1) / 2); }
};
