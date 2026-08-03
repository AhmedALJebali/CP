struct Node {
  int val, real;
  Node(){ val = real = 0 ; }
  void change(int x, int r) {
    val = x;
    real = r;
  }
};

struct segment_tree {
  int tree_size;
  vector<Node> segData;
  segment_tree(int n) {
    tree_size = 1;
    while (tree_size < n) tree_size *= 2;
    segData.assign(2 * tree_size, Node());
  }
  Node merge(Node &left, Node &right) {
    Node res;
    res.val = left.val ^ right.val;
    res.real = left.real + right.real;
    return res;
  }
  Node get(int l, int r, int node, int lx, int rx) {
    if (lx >= l && rx <= r) return segData[node];
    if (rx <= l || lx >= r) return Node();
    int mid = (lx + rx) / 2;
    Node left = get(l, r, 2 * node + 1, lx, mid);
    Node right = get(l, r, 2 * node + 2, mid, rx);
    return merge(left, right);
  }
  void update(int idx, int val, int r, int node, int lx, int rx) {
    if (rx - lx == 1) {
      segData[node].change(val, r);
      return;
    }
    int mid = (lx + rx) / 2;
    if (idx < mid) {
      update(idx, val, r, 2 * node + 1, lx, mid);
    } else {
      update(idx, val, r, 2 * node + 2, mid, rx);
    }
    segData[node] = merge(segData[2 * node + 1], segData[2 * node + 2]);
  }
  int kth(int k, int node, int lx, int rx) {
      if (segData[node].real < k) {
        return -1;
      }
      if (rx - lx == 1) return lx;
      int mid = (lx + rx) / 2;
      if (segData[2 * node + 1].real >= k) {
        return kth(k, 2 * node + 1, lx, mid);
      } else {
        return kth(k - segData[2 * node + 1].real, 2 * node + 2, mid, rx);
      }
  }
  void update(int i, int val, int r) {
    update(i, val, r, 0, 0, tree_size);
  }
  void remove(int k) {
    update(kth(k, 0, 0, tree_size), 0, 0);
  }
  int get_active(int l, int r) {
    return get(kth(l, 0, 0, tree_size), kth(r, 0, 0, tree_size) + 1, 0, 0, tree_size).val;
  }
};

void solve() {
  int n; cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  int q; cin >> q;
  vector<array<int,3>> qu(q);
  for (int i = 0; i < q; i++) {
    cin >> qu[i][0];
    if (qu[i][0] == 1) {
      cin >> qu[i][1] >> qu[i][2];
    } else if (qu[i][0] == 2) {
      cin >> qu[i][1];
    } else {
      cin >> qu[i][1] >> qu[i][2];
    }
  }

  list<int> order;
  vector<list<int>::iterator> active;
  int id_counter = 0;

  vector<int> initial_ids(n);
  for (int i = 0; i < n; i++) {
    initial_ids[i] = id_counter++;
    order.push_back(initial_ids[i]);
    active.push_back(--order.end());
  }

  vector<int> ids(q, -1);
  for (int i = 0; i < q; i++) {
    if (qu[i][0] == 1) {
      int idx = qu[i][1] - 1;
      int new_id = id_counter++;
      ids[i] = new_id;
      if (idx == active.size()) {
        order.push_back(new_id);
        active.push_back(--order.end());
      } else {
        auto it = active[idx];
        order.insert(it, new_id);
        active.insert(active.begin() + idx, --it);
      }
    } else if (qu[i][0] == 2) {
      int idx = qu[i][1] - 1;
      active.erase(active.begin() + idx);
    }
  }
  vector<int> idx(id_counter);
  int cur = 0;
  for (int id : order) idx[id] = cur++;
  segment_tree seg(cur + 5);
  for (int i = 0; i < n; i++) {
    seg.update(idx[initial_ids[i]], a[i], 1);
  }
  for (int i = 0; i < q; i++) {
    int t = qu[i][0];
    if (t == 1) {
      int id = ids[i];
      int val = qu[i][2];
      seg.update(idx[id], val, 1);
    }
    else if (t == 2) {
      int idx = qu[i][1];
      seg.remove(idx);
    }
    else {
      int l = qu[i][1];
      int r = qu[i][2];
      cout << seg.get_active(l, r) << "\n";
    }
  }
}
