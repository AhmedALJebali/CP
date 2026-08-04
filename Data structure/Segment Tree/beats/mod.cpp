// sum from l r 
// apply mod 
// assign value
// Modulo operation l, r, x. Picks should perform assignment a[i] = a[i] mod x for each i (l ≤ i ≤ r).
// Set operation k, x. Picks should set the value of a[k] to x (in other words perform an assignment a[k] = x).

const int N = 100005;
struct Node {
  int sum;
  int mx;
} tree[N * 4];
void build(int node, int lx, int rx, const vector<int>& a) {
  if (rx - lx == 1) {
    tree[node].sum = a[lx];
    tree[node].mx = a[lx];
    return;
  }
  int mid = lx + (rx - lx) / 2;
  build(2 * node, lx, mid, a);
  build(2 * node + 1, mid, rx, a);

  tree[node].sum = tree[2 * node].sum + tree[2 * node + 1].sum;
  tree[node].mx = max(tree[2 * node].mx, tree[2 * node + 1].mx);
}
 
void upd_set(int node, int i, int x, int lx, int rx) {
  if (rx - lx == 1) {
    tree[node].sum = x;
    tree[node].mx = x;
    return;
  }
  int mid = lx + (rx - lx) / 2;
  if (i < mid) {
    upd_set(2 * node, i, x, lx, mid);
  } else {
    upd_set(2 * node + 1, i, x, mid, rx);
  }
 
  tree[node].sum = tree[2 * node].sum + tree[2 * node + 1].sum;
  tree[node].mx = max(tree[2 * node].mx, tree[2 * node + 1].mx);
}
void upd_mod(int node, int l, int r, int x, int lx, int rx) {
  if (rx <= l || r <= lx || tree[node].mx < x) return;
  if (rx - lx == 1) {
    tree[node].sum %= x;
    tree[node].mx %= x;
    return;
  }
  int mid = lx + (rx - lx) / 2;
  upd_mod(2 * node, l, r, x, lx, mid);
  upd_mod(2 * node + 1, l, r, x, mid, rx);
  tree[node].sum = tree[2 * node].sum + tree[2 * node + 1].sum;
  tree[node].mx = max(tree[2 * node].mx, tree[2 * node + 1].mx);
}
int get(int node, int l, int r, int lx, int rx) {
  if (rx <= l || r <= lx) return 0;
  if (l <= lx && rx <= r) return tree[node].sum;
  int mid = lx + (rx - lx) / 2;
  return get(2 * node, l, r, lx, mid) + get(2 * node + 1, l, r, mid, rx);
}
void solve() {
  int n, q;
  cin >> n >> q;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  build(1, 0, n, a);
  while (q--) {
    int t, l, r;
    cin >> t >> l >> r;
    l--;
    if (t == 1) {
      cout << get(1, l, r, 0, n) << '\n';
    } else if (t == 2) {
      int x;
      cin >> x;
      upd_mod(1, l, r, x, 0, n);
    } else {
      upd_set(1, l, r, 0, n);
    }
  }
}
 
