const int N = 1e5 + 5;
const int MAX_NODES = 6000000;
 
struct Node {
    Node *l, *r;
    int sum;
    Node(int x = 0) : l(nullptr), r(nullptr), sum(x) {}
};
 
Node pool[MAX_NODES];
 
 
Node* upd(Node* root, int i, int x, int lx = 0, int rx = N) {
    if (rx - lx == 1) {
        return new Node((root ? root->sum : 0) + x);
    }
    int mid = lx + (rx - lx) / 2;
    Node* cur = new Node();
    if (root) {
        cur->l = root->l;
        cur->r = root->r;
    }
    if (i < mid) {
        cur->l = upd(root ? root->l : nullptr, i, x, lx, mid);
    } else {
        cur->r = upd(root ? root->r : nullptr, i, x, mid, rx);
    }
    cur->sum = (cur->l ? cur->l->sum : 0) + (cur->r ? cur->r->sum : 0);
    return cur;
}
 
int get_kth(Node* root, int k, int lx = 0, int rx = N) {
    if (!root || root->sum < k) return rx;
    if (rx - lx == 1) return lx;
 
    int mid = lx + (rx - lx) / 2;
    int left_sum = root->l ? root->l->sum : 0;
 
    if (k <= left_sum) {
        return get_kth(root->l, k, lx, mid);
    } else {
        return get_kth(root->r, k - left_sum, mid, rx);
    }
}
void solve() {
    int n;
    cin>>n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    vector<Node*> ver(n + 2, nullptr);
    vector<int> nxt(N, 0);
    for (int i = n; i >= 1; i--) {
        Node* cur = ver[i + 1];
        if (nxt[a[i]] != 0) {
            cur = upd(cur, nxt[a[i]], -1);
        }
        cur = upd(cur, i, 1);
        ver[i] = cur;
        nxt[a[i]] = i;
    }
    for (int k = 1; k <= n; k++) {
        int ans = 0;
        int L = 1;
        while (L <= n) {
            ans++;
            L = get_kth(ver[L], k + 1);
        }
        cout << ans << " ";
    }
    cout << "\n";
}
 
