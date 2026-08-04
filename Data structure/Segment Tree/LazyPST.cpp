const int N = 1e5;
int a[N + 5];
struct Node {
  int l, r;
  long long sum, lazy;  // Only these need to be 64-bit
  bool hasLazy;
  Node(int  x = 0) {
    sum = x;
    l = r = lazy = 0;
    hasLazy = 0;
  }
  void change(int  x, int lx, int rx) {
    sum += 1LL * (rx - lx) * x;
    lazy += x;
    hasLazy = 1;
  }
} tree[N * 100];

int id = 0;
void pull(int cur_id) {
  tree[cur_id].sum = tree[tree[cur_id].l].sum + tree[tree[cur_id].r].sum;
}
int push(int pre, int lx, int rx) {
  int cur_id = ++id;
  tree[cur_id] = tree[pre];
  if (tree[cur_id].hasLazy) {
    if (rx - lx > 1) {
      int lc = ++id, rc = ++id;
      tree[lc] = tree[tree[pre].l];
      tree[rc] = tree[tree[pre].r];
      int mid = lx + (rx - lx) / 2;
      tree[lc].change(tree[cur_id].lazy, lx, mid);
      tree[rc].change(tree[cur_id].lazy, mid, rx);
      tree[cur_id].l = lc;
      tree[cur_id].r = rc;
    }
    tree[cur_id].lazy = 0;
    tree[cur_id].hasLazy = 0;
  }
  return cur_id;
}

int upd(int node, int l, int r, long long x, int lx, int rx) {
  if (rx <= l || r <= lx) return node;
  if (l <= lx && rx <= r) {
    int cur_id = ++id;
    tree[cur_id] = tree[node];
    tree[cur_id].change(x, lx, rx);
    return cur_id;
  }
  int cur_id = push(node, lx, rx);
  int mid = lx + (rx - lx) / 2;
  tree[cur_id].l = upd(tree[cur_id].l, l, r, x, lx, mid);
  tree[cur_id].r = upd(tree[cur_id].r, l, r, x, mid, rx);
  pull(cur_id);
  return cur_id;
}
int get(int node, int l, int r, int lx, int rx) {
  if (rx <= l || r <= lx) return 0;
  if (l <= lx && rx <= r) {
    return tree[node].sum ;
  }
  node=push(node,lx,rx);
  int mid = lx + (rx - lx) / 2;
  return get(tree[node].l, l, r, lx, mid) + get(tree[node].r, l, r, mid, rx);
}
int build(int lx, int rx) {
  int cur_id = ++id;
  if (rx - lx == 1) {
    tree[cur_id].sum = a[lx];
    return cur_id;
  }
  int mid = lx + (rx - lx) / 2;
  tree[cur_id].l = build(lx, mid);
  tree[cur_id].r = build(mid, rx);
  pull(cur_id);
  return cur_id;
}
void solve() {
  int n, q;
  cin>>n>>q;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  int st = build(0, n);
  vector<int> time(q + 5, 0);
  time[0] = st;
  int t = 0;
  while (q--) {
    char typo;
    cin >> typo;
    if (typo == 'Q') {
      int l, r;
      cin >> l >> r;
      l--;
      cout << get(time[t], l, r, 0, n) << "\n";
    } else if (typo == 'C') {
      int l, r;
      long long d;
      cin >> l >> r >> d;
      l--;
      time[t + 1] = upd(time[t], l, r, d, 0, n);
      t++;
    } else if (typo == 'B') {
      int back;
      cin >> back;
      t = back;
    } else if (typo == 'H') {
      int l, r, tt;
      cin >> l >> r >> tt;
      l--;
      cout << get(time[tt], l, r, 0, n) << "\n";
    }
  }
}
