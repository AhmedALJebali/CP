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
struct OfflineLayout {
  // Total number of unique elements ever inserted.
  int id_counter = 0;

  // Maintains the relative chronological order of all elements ever added.
  // Even if an element is "removed", it stays in this list to reserve its spot in the static layout.
  list<int> order;

  // Represents the current "live" state of the array.
  // Maps a current dynamic index to its node in the `order` list.
  vector<list<int>::iterator> active;

  // Stores the final mapped index for every unique element ID.
  vector<int> mapped_index;

  // Constructor: Pre-fills the structure with `initial_size` elements.
  OfflineLayout(int initial_size = 0) {
    for (int i = 0; i < initial_size; i++) {
      int id = id_counter++;
      order.push_back(id);
      active.push_back(--order.end());
    }
  }

  // Simulates inserting an element at dynamic index `pos`.
  // Returns a unique identifier for this specific insertion.
  int insert(int pos) {
    int new_id = id_counter++;
    if (pos == active.size()) {
      // Append to the end
      order.push_back(new_id);
      active.push_back(--order.end());
    } else {
      // Insert in the middle.
      // Note: active.insert() takes O(V) time where V is the current number of active elements.
      auto it = active[pos];
      auto new_it = order.insert(it, new_id);
      active.insert(active.begin() + pos, new_it);
    }
    return new_id;
  }

  // Simulates removing the element currently at dynamic index `pos`.
  // Note: It is only removed from `active`. It remains in `order`.
  void remove(int pos) {
    active.erase(active.begin() + pos);
  }

  // Finalizes the static layout. MUST be called after all inserts/removes
  // and BEFORE calling id(identifier).
  void build() {
    mapped_index.assign(id_counter, -1);
    int cur = 0;
    for (int id : order) {
      mapped_index[id] = cur++;
    }
  }

  // Returns the static 0-based array index for a given element identifier.
  int id(int identifier) {
    return mapped_index[identifier];
  }

  // Returns the total number of unique elements (the required size for your Segment Tree).
  int size() {
    return id_counter;
  }
};
void solve() {
  int n; cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  OfflineLayout idx(n);
  int q; cin >> q;
  vector<array<int, 3>> qu(q);
  for (int i=0;i<q;i++) {
    int t,pos,val;cin>>t;
    if (t==1) {
      cin>>pos>>val;pos--;
      pos=idx.insert(pos);
      qu[i]= {t,pos,val};
    }else if (t==2) {
      cin>>pos;pos--;
      idx.remove(pos);
      qu[i]= {t,pos+1,0};
    } else {
      int l,r; cin>>l>>r;
      qu[i]= {t,l,r};
    }
  }
  idx.build();
  segment_tree seg(idx.size()+10);
  for (int i=0;i<n;i++)seg.update(idx.id(i),a[i],1);
  for (int i=0;i<q;i++) {
    auto [t,i1,i2]=qu[i];
    if (t==1) {
      seg.update(idx.id(i1),i2,1);
    }else if (t==2) {
      seg.remove(i1);
    } else {
      cout<<seg.get_active(i1,i2)<<endl;
    }
  }
}
