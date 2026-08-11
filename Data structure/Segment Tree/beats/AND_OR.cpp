static constexpr int INF = (1LL << 60);

static constexpr int B = 20;
static constexpr int MASK = (1 << B) - 1;

struct SegTreeBeats {
    int n;

    // mx  = maximum value in segment
    // band = bitwise AND of all values
    // bor  = bitwise OR  of all values
    vector<int> mx, band, bor;

    // Pending transformation:
    // v -> (v & lazyAnd) | lazyOr
    vector<int> lazyAnd, lazyOr;

    SegTreeBeats(const vector<int>& a) {
        n = (int)a.size();

        int sz = 4 * n + 5;
        mx.resize(sz);
        band.resize(sz);
        bor.resize(sz);

        lazyAnd.assign(sz, MASK);
        lazyOr.assign(sz, 0);

        build(1, 0, n - 1, a);
    }

    void build(int p, int l, int r, const vector<int>& a) {
        if (l == r) {
            mx[p] = band[p] = bor[p] = a[l];
            return;
        }

        int m = (l + r) >> 1;

        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);

        pull(p);
    }

    void pull(int p) {
        int lc = p << 1;
        int rc = lc | 1;

        mx[p] = max(mx[lc], mx[rc]);
        band[p] = band[lc] & band[rc];
        bor[p] = bor[lc] | bor[rc];
    }

    // Apply arbitrary bit transformation:
    // v -> (v & a) | o
    void apply(int p, int a, int o) {
        mx[p]   = (mx[p]   & a) | o;
        band[p] = (band[p] & a) | o;
        bor[p]  = (bor[p]  & a) | o;

        // Compose:
        // old f(v) = (v & lazyAnd) | lazyOr
        // new g(x) = (x & a) | o
        //
        // g(f(v))
        // = (((v & lazyAnd) | lazyOr) & a) | o
        // = (v & lazyAnd & a) | (lazyOr & a) | o
        lazyAnd[p] &= a;
        lazyOr[p] = (lazyOr[p] & a) | o;
    }

    void push(int p) {
        if (lazyAnd[p] == MASK && lazyOr[p] == 0)
            return;

        int lc = p << 1;
        int rc = lc | 1;

        apply(lc, lazyAnd[p], lazyOr[p]);
        apply(rc, lazyAnd[p], lazyOr[p]);

        lazyAnd[p] = MASK;
        lazyOr[p] = 0;
    }

    /*
        For AND X:

        Bits removed are ~X.

        If a bit is mixed inside this node:
            band bit = 0
            bor  bit = 1

        Then we cannot safely update the whole node.

        mixed = band ^ bor

        So whole-node AND is possible iff:
            (mixed & (~X)) == 0
    */
    bool canApplyAnd(int p, int x) const {
        int removedBits = MASK ^ x;
        return ((band[p] ^ bor[p]) & removedBits) == 0;
    }

    /*
        For OR X:

        Bits added are X.

        Whole-node OR is possible iff all bits in X are uniform.
    */
    bool canApplyOr(int p, int x) const {
        return ((band[p] ^ bor[p]) & x) == 0;
    }

    void rangeAnd(int ql, int qr, int x) {
        rangeAnd(1, 0, n - 1, ql, qr, x);
    }

    void rangeAnd(int p, int l, int r,
                  int ql, int qr, int x) {

        if (qr < l || r < ql)
            return;

        if (ql <= l && r <= qr) {
            // If this operation doesn't change anything,
            // no need to descend.
            if ((bor[p] & (~x & MASK)) == 0)
                return;

            // Safe whole-node update.
            if (canApplyAnd(p, x)) {
                apply(p, x, 0);
                return;
            }
        }

        // Leaf: always safe.
        if (l == r) {
            apply(p, x, 0);
            return;
        }

        push(p);

        int m = (l + r) >> 1;

        rangeAnd(p << 1, l, m, ql, qr, x);
        rangeAnd(p << 1 | 1, m + 1, r, ql, qr, x);

        pull(p);
    }

    void rangeOr(int ql, int qr, int x) {
        rangeOr(1, 0, n - 1, ql, qr, x);
    }

    void rangeOr(int p, int l, int r,
                 int ql, int qr, int x) {

        if (qr < l || r < ql)
            return;

        if (ql <= l && r <= qr) {
            // If every bit in x is already 1 everywhere,
            // OR changes nothing.
            if ((band[p] & x) == x)
                return;

            // Safe whole-node update.
            if (canApplyOr(p, x)) {
                apply(p, MASK, x);
                return;
            }
        }

        // Leaf: always safe.
        if (l == r) {
            apply(p, MASK, x);
            return;
        }

        push(p);

        int m = (l + r) >> 1;

        rangeOr(p << 1, l, m, ql, qr, x);
        rangeOr(p << 1 | 1, m + 1, r, ql, qr, x);

        pull(p);
    }

    int rangeMax(int ql, int qr) {
        return rangeMax(1, 0, n - 1, ql, qr);
    }

    int rangeMax(int p, int l, int r,
                 int ql, int qr) {

        if (qr < l || r < ql)
            return -1;

        if (ql <= l && r <= qr)
            return mx[p];

        push(p);

        int m = (l + r) >> 1;

        return max(
            rangeMax(p << 1, l, m, ql, qr),
            rangeMax(p << 1 | 1, m + 1, r, ql, qr)
        );
    }
};

