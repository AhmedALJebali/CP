const int N = 200005;

struct Node {
    int sum;
    int mn;
    int mx;
    int l_val;
    int r_val;
    int gcd_diff;
    int lazy_add;
    int lazy_assign;
    bool has_assign;
} tree[N * 4];
 
void push_up(int node) {
    int l = 2 * node, r = 2 * node + 1;
    tree[node].sum = tree[l].sum + tree[r].sum;
    tree[node].mn = min(tree[l].mn, tree[r].mn);
    tree[node].mx = max(tree[l].mx, tree[r].mx);
    tree[node].l_val = tree[l].l_val;
    tree[node].r_val = tree[r].r_val;
    tree[node].gcd_diff =gcd(gcd(tree[l].gcd_diff, tree[r].gcd_diff),
                                   abs(tree[r].l_val - tree[l].r_val));
}
 
void apply_assign(int node, int x, int lx, int rx) {
    int len = rx - lx;
    tree[node].sum = x * len;
    tree[node].mn = x;
    tree[node].mx = x;
    tree[node].l_val = x;
    tree[node].r_val = x;
    tree[node].gcd_diff = 0;
    tree[node].lazy_assign = x;
    tree[node].has_assign = true;
    tree[node].lazy_add = 0;
}
 
void apply_add(int node, int x, int lx, int rx) {
    int len = rx - lx;
    tree[node].sum += x * len;
    tree[node].mn += x;
    tree[node].mx += x;
    tree[node].l_val += x;
    tree[node].r_val += x;
    if (tree[node].has_assign) {
        tree[node].lazy_assign += x;
    } else {
        tree[node].lazy_add += x;
    }
}
 
void push_down(int node, int lx, int rx) {
    int mid = lx + (rx - lx) / 2;
    if (tree[node].has_assign) {
        apply_assign(2 * node, tree[node].lazy_assign, lx, mid);
        apply_assign(2 * node + 1, tree[node].lazy_assign, mid, rx);
        tree[node].has_assign = false;
    }
    if (tree[node].lazy_add != 0) {
        apply_add(2 * node, tree[node].lazy_add, lx, mid);
        apply_add(2 * node + 1, tree[node].lazy_add, mid, rx);
        tree[node].lazy_add = 0;
    }
}
 
void build(int node, int lx, int rx, const vector<int>& a) {
    tree[node].lazy_add = 0;
    tree[node].has_assign = false;
    if (rx - lx == 1) {
        tree[node].sum = tree[node].mn = tree[node].mx = a[lx];
        tree[node].l_val = tree[node].r_val = a[lx];
        tree[node].gcd_diff = 0;
        return;
    }
    int mid = lx + (rx - lx) / 2;
    build(2 * node, lx, mid, a);
    build(2 * node + 1, mid, rx, a);
    push_up(node);
}
 
void upd_mn(int node, int l, int r, int x, int lx, int rx) {
    if (rx <= l || r <= lx || tree[node].mx <= x) return;
    if (l <= lx && rx <= r && tree[node].mn >= x) {
        apply_assign(node, x, lx, rx);
        return;
    }
    push_down(node, lx, rx);
    int mid = lx + (rx - lx) / 2;
    upd_mn(2 * node, l, r, x, lx, mid);
    upd_mn(2 * node + 1, l, r, x, mid, rx);
    push_up(node);
}
 
void upd_mx(int node, int l, int r, int x, int lx, int rx) {
    if (rx <= l || r <= lx || tree[node].mn >= x) return;
    if (l <= lx && rx <= r && tree[node].mx <= x) {
        apply_assign(node, x, lx, rx);
        return;
    }
    push_down(node, lx, rx);
    int mid = lx + (rx - lx) / 2;
    upd_mx(2 * node, l, r, x, lx, mid);
    upd_mx(2 * node + 1, l, r, x, mid, rx);
    push_up(node);
}
 
void upd_assign(int node, int l, int r, int x, int lx, int rx) {
    if (rx <= l || r <= lx) return;
    if (l <= lx && rx <= r) {
        apply_assign(node, x, lx, rx);
        return;
    }
    push_down(node, lx, rx);
    int mid = lx + (rx - lx) / 2;
    upd_assign(2 * node, l, r, x, lx, mid);
    upd_assign(2 * node + 1, l, r, x, mid, rx);
    push_up(node);
}
 
void upd_add(int node, int l, int r, int x, int lx, int rx) {
    if (rx <= l || r <= lx) return;
    if (l <= lx && rx <= r) {
        apply_add(node, x, lx, rx);
        return;
    }
    push_down(node, lx, rx);
    int mid = lx + (rx - lx) / 2;
    upd_add(2 * node, l, r, x, lx, mid);
    upd_add(2 * node + 1, l, r, x, mid, rx);
    push_up(node);
}
 
int get_sum(int node, int l, int r, int lx, int rx) {
    if (rx <= l || r <= lx) return 0;
    if (l <= lx && rx <= r) return tree[node].sum;
    push_down(node, lx, rx);
    int mid = lx + (rx - lx) / 2;
    return get_sum(2 * node, l, r, lx, mid) + get_sum(2 * node + 1, l, r, mid, rx);
}
 
int get_min(int node, int l, int r, int lx, int rx) {
    if (rx <= l || r <= lx) return 9e18;
    if (l <= lx && rx <= r) return tree[node].mn;
    push_down(node, lx, rx);
    int mid = lx + (rx - lx) / 2;
    return min(get_min(2 * node, l, r, lx, mid), get_min(2 * node + 1, l, r, mid, rx));
}
 
int get_max(int node, int l, int r, int lx, int rx) {
    if (rx <= l || r <= lx) return -9e18;
    if (l <= lx && rx <= r) return tree[node].mx;
    push_down(node, lx, rx);
    int mid = lx + (rx - lx) / 2;
    return max(get_max(2 * node, l, r, lx, mid), get_max(2 * node + 1, l, r, mid, rx));
}
 
Node get_gcd(int node, int l, int r, int lx, int rx) {
    if (l <= lx && rx <= r) {
        return tree[node];
    }
    push_down(node, lx, rx);
    int mid = lx + (rx - lx) / 2;
 
    if (r <= mid) return get_gcd(2 * node, l, r, lx, mid);
    if (l >= mid) return get_gcd(2 * node + 1, l, r, mid, rx);
 
    Node left_res = get_gcd(2 * node, l, r, lx, mid);
    Node right_res = get_gcd(2 * node + 1, l, r, mid, rx);
    Node res;
    res.l_val = left_res.l_val;
    res.r_val = right_res.r_val;
    res.gcd_diff = gcd(gcd(left_res.gcd_diff, right_res.gcd_diff), abs(right_res.l_val - left_res.r_val));
    return res;
}
 
void solve() {
    int n;
    cin>>n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
 
    int q;
    cin >> q;
    build(1, 0, n, a);
    while (q--) {
        int t, l, r;
        cin >> t >> l >> r;
        l--;
        if (t <= 4) {
            int x;
            cin >> x;
            if (t == 1) upd_mn(1, l, r, x, 0, n);
            else if (t == 2) upd_mx(1, l, r, x, 0, n);
            else if (t == 3) upd_assign(1, l, r, x, 0, n);
            else if (t == 4) upd_add(1, l, r, x, 0, n);
        } else {
            if (t == 5) cout << get_sum(1, l, r, 0, n) << '\n';
            else if (t == 6) cout << get_min(1, l, r, 0, n) << '\n';
            else if (t == 7) cout << get_max(1, l, r, 0, n) << '\n';
            else if (t == 8) {
                Node res = get_gcd(1, l, r, 0, n);
                cout << gcd(res.l_val, res.gcd_diff) << '\n';
            }
        }
    }
}
 
