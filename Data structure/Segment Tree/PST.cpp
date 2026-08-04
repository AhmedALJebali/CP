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

