    Node get(int l, int r, int node, int lx, int rx) {
        propagate(node, lx, rx);
        if (lx >= l && rx <= r) return segData[node];
        int mid = (lx + rx) / 2;
        bool go_left = (l < mid);
        bool go_right = (r > mid);
        if (go_left && go_right) {
            Node left = get(l, r, 2 * node + 1, lx, mid);
            Node right = get(l, r, 2 * node + 2, mid, rx);
            return merge(left, right);
        } else if (go_left) {
            return get(l, r, 2 * node + 1, lx, mid);
        } else {
            return get(l, r, 2 * node + 2, mid, rx);
        }
    }
