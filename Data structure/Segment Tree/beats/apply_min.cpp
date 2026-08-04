// apply min on val

const int N = 300005;
struct Node {
    int sum;
    int mx1;
    int mx2;
    int mx_cnt;
} tree[N * 4];

void push_up(int node) {
    int l = 2 * node, r = 2 * node + 1;
    tree[node].sum = tree[l].sum + tree[r].sum;
    if (tree[l].mx1 == tree[r].mx1) {
        tree[node].mx1 = tree[l].mx1;
        tree[node].mx_cnt = tree[l].mx_cnt + tree[r].mx_cnt;
        tree[node].mx2 = max(tree[l].mx2, tree[r].mx2);
    } else if (tree[l].mx1 > tree[r].mx1) {
        tree[node].mx1 = tree[l].mx1;
        tree[node].mx_cnt = tree[l].mx_cnt;
        tree[node].mx2 = max(tree[l].mx2, tree[r].mx1);
    } else {
        tree[node].mx1 = tree[r].mx1;
        tree[node].mx_cnt = tree[r].mx_cnt;
        tree[node].mx2 = max(tree[l].mx1, tree[r].mx2);
    }
}

void apply(int node, int x) {
    if (tree[node].mx1 <= x) return;
    tree[node].sum -= (tree[node].mx1 - x) * tree[node].mx_cnt;
    tree[node].mx1 = x;
}

void push_down(int node) {
    apply(2 * node, tree[node].mx1);
    apply(2 * node + 1, tree[node].mx1);
}

void build(int node, int lx, int rx, const vector<int>& a) {
    if (rx - lx == 1) {
        tree[node] = {a[lx], a[lx], -1, 1};
        return;
    }
    int mid = lx + (rx - lx) / 2;
    build(2 * node, lx, mid, a);
    build(2 * node + 1, mid, rx, a);
    push_up(node);
}

void upd_mn(int node, int l, int r, int x, int lx, int rx) {
    if (rx <= l || r <= lx || tree[node].mx1 <= x) return;
    if (l <= lx && rx <= r && tree[node].mx2 < x) {
        apply(node, x);
        return;
    }
    push_down(node);
    int mid = lx + (rx - lx) / 2;
    upd_mn(2 * node, l, r, x, lx, mid);
    upd_mn(2 * node + 1, l, r, x, mid, rx);
    push_up(node);
}

int get(int node, int l, int r, int lx, int rx) {
    if (rx <= l || r <= lx) return 0;
    if (l <= lx && rx <= r) return tree[node].sum;
    push_down(node);
    int mid = lx + (rx - lx) / 2;
    return get(2 * node, l, r, lx, mid) + get(2 * node + 1, l, r, mid, rx);
}
void solve() {
  int n, q;
  cin >> n ;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  cin>>q;
  build(1, 0, n, a);
  while (q--) {
    int t, l, r;
    cin >> t >> l >> r;
    l--;
    if (t == 2) {
      cout << get(1, l, r, 0, n) << '\n';
    } else {
      int x;
      cin >> x;
      upd_mn(1, l, r, x, 0, n);
    }
  }
}
