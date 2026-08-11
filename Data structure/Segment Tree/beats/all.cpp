static constexpr int INF = (1LL << 60);
 
// ================================================================
// PASTE YOUR SegmentTreeBeats HERE
// ================================================================
 
struct SegmentTreeBeats {
 
    struct Node {
        int sum = 0;
 
        // ========================================================
        // Minimum
        // ========================================================
        int mn1 = INF;
        int mn2 = INF;
        int cntMn = 0;
 
        // ========================================================
        // Maximum
        // ========================================================
        int mx1 = -INF;
        int mx2 = -INF;
        int cntMx = 0;
 
        // ========================================================
        // Lazy operations
        // ========================================================
        int add = 0;
 
        // INF = no assignment
        int set = INF;
    };
 
    int n;
    vector<Node> st;
 
    // ============================================================
    // Constructor
    // ============================================================
 
    SegmentTreeBeats(const vector<int>& a) {
        n = (int)a.size();
        st.resize(4 * n);
        build(1, 0, n - 1, a);
    }
 
    // ============================================================
    // APPLY SET
    // ============================================================
 
    void apply_set(int p, int l, int r, int x) {
        Node &t = st[p];
 
        int len = r - l + 1LL;
 
        t.sum = x * len;
 
        t.mn1 = x;
        t.mn2 = INF;
        t.cntMn = (int)len;
 
        t.mx1 = x;
        t.mx2 = -INF;
        t.cntMx = (int)len;
 
        t.set = x;
        t.add = 0;
    }
 
    // ============================================================
    // APPLY ADD
    // ============================================================
 
    void apply_add(int p, int l, int r, int x) {
        Node &t = st[p];
 
        int len = r - l + 1LL;
 
        t.sum += x * len;
 
        t.mn1 += x;
 
        if (t.mn2 != INF)
            t.mn2 += x;
 
        t.mx1 += x;
 
        if (t.mx2 != -INF)
            t.mx2 += x;
 
        // Composition:
        //
        // set(v) + x = set(v + x)
        //
        // otherwise accumulate add.
        if (t.set != INF)
            t.set += x;
        else
            t.add += x;
    }
 
    // ============================================================
    // APPLY CHMIN
    //
    // Valid when:
    //
    //      mx2 < x < mx1
    //
    // Only elements equal to mx1 change.
    // ============================================================
 
    void apply_chmin(int p, int x) {
        Node &t = st[p];
 
        if (t.mx1 <= x)
            return;
 
        int oldMx = t.mx1;
 
        t.sum -= (oldMx - x) * 1LL * t.cntMx;
 
        // If aint values were equal.
        if (t.mn1 == oldMx) {
            t.mn1 = x;
        }
        // Old maximum becomes second minimum.
        else if (t.mn2 == oldMx) {
            t.mn2 = x;
        }
 
        t.mx1 = x;
 
        // A uniform assigned segment remains assigned.
        if (t.set != INF)
            t.set = x;
    }
 
    // ============================================================
    // APPLY CHMAX
    //
    // Valid when:
    //
    //      mn1 < x < mn2
    //
    // Only elements equal to mn1 change.
    // ============================================================
 
    void apply_chmax(int p, int x) {
        Node &t = st[p];
 
        if (t.mn1 >= x)
            return;
 
        int oldMn = t.mn1;
 
        t.sum += (x - oldMn) * 1LL * t.cntMn;
 
        // If aint values were equal.
        if (t.mx1 == oldMn) {
            t.mx1 = x;
        }
        // Old minimum becomes second maximum.
        else if (t.mx2 == oldMn) {
            t.mx2 = x;
        }
 
        t.mn1 = x;
 
        if (t.set != INF)
            t.set = x;
    }
 
    // ============================================================
    // APPLY MOD
    //
    // Modulo cannot be represented by a single lazy value.
    //
    // So this function is only used on a leaf.
    // ============================================================
 
    void apply_mod(int p, int x) {
        Node &t = st[p];
 
        int v = t.mx1 % x;
 
        t.sum = v;
 
        t.mn1 = v;
        t.mn2 = INF;
        t.cntMn = 1;
 
        t.mx1 = v;
        t.mx2 = -INF;
        t.cntMx = 1;
 
        t.add = 0;
        t.set = INF;
    }
 
    // ============================================================
    // PULL
    // ============================================================
 
    void puint(int p) {
        Node &t = st[p];
        const Node &L = st[p << 1];
        const Node &R = st[p << 1 | 1];
 
        t.sum = L.sum + R.sum;
 
        // --------------------------------------------------------
        // Minimum
        // --------------------------------------------------------
 
        if (L.mn1 < R.mn1) {
            t.mn1 = L.mn1;
            t.cntMn = L.cntMn;
            t.mn2 = min(L.mn2, R.mn1);
        }
        else if (L.mn1 > R.mn1) {
            t.mn1 = R.mn1;
            t.cntMn = R.cntMn;
            t.mn2 = min(L.mn1, R.mn2);
        }
        else {
            t.mn1 = L.mn1;
            t.cntMn = L.cntMn + R.cntMn;
            t.mn2 = min(L.mn2, R.mn2);
        }
 
        // --------------------------------------------------------
        // Maximum
        // --------------------------------------------------------
 
        if (L.mx1 > R.mx1) {
            t.mx1 = L.mx1;
            t.cntMx = L.cntMx;
            t.mx2 = max(L.mx2, R.mx1);
        }
        else if (L.mx1 < R.mx1) {
            t.mx1 = R.mx1;
            t.cntMx = R.cntMx;
            t.mx2 = max(L.mx1, R.mx2);
        }
        else {
            t.mx1 = L.mx1;
            t.cntMx = L.cntMx + R.cntMx;
            t.mx2 = max(L.mx2, R.mx2);
        }
 
        // Children are now represented by the parent.
        t.add = 0;
        t.set = INF;
    }
 
    // ============================================================
    // PUSH
    // ============================================================
 
    void push(int p, int l, int r) {
        if (l == r)
            return;
 
        int m = (l + r) >> 1;
 
        int lc = p << 1;
        int rc = lc | 1;
 
        Node &t = st[p];
 
        // --------------------------------------------------------
        // 1. SET
        // --------------------------------------------------------
 
        if (t.set != INF) {
            apply_set(lc, l, m, t.set);
            apply_set(rc, m + 1, r, t.set);
 
            t.set = INF;
        }
 
        // --------------------------------------------------------
        // 2. ADD
        // --------------------------------------------------------
 
        if (t.add != 0) {
            apply_add(lc, l, m, t.add);
            apply_add(rc, m + 1, r, t.add);
 
            t.add = 0;
        }
 
        // --------------------------------------------------------
        // 3. CHMIN constraint
        // --------------------------------------------------------
 
        if (st[lc].mx1 > t.mx1)
            apply_chmin(lc, t.mx1);
 
        if (st[rc].mx1 > t.mx1)
            apply_chmin(rc, t.mx1);
 
        // --------------------------------------------------------
        // 4. CHMAX constraint
        // --------------------------------------------------------
 
        if (st[lc].mn1 < t.mn1)
            apply_chmax(lc, t.mn1);
 
        if (st[rc].mn1 < t.mn1)
            apply_chmax(rc, t.mn1);
    }
 
    // ============================================================
    // BUILD
    // ============================================================
 
    void build(int p, int l, int r, const vector<int>& a) {
 
        if (l == r) {
            Node &t = st[p];
 
            t.sum = a[l];
 
            t.mn1 = a[l];
            t.mn2 = INF;
            t.cntMn = 1;
 
            t.mx1 = a[l];
            t.mx2 = -INF;
            t.cntMx = 1;
 
            t.add = 0;
            t.set = INF;
 
            return;
        }
 
        int m = (l + r) >> 1;
 
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
 
        puint(p);
    }
 
    // ============================================================
    // RANGE CHMIN
    // ============================================================
 
    void range_chmin(
        int p,
        int l,
        int r,
        int ql,
        int qr,
        int x
    ) {
        if (r < ql || qr < l || st[p].mx1 <= x)
            return;
 
        if (ql <= l &&
            r <= qr &&
            st[p].mx2 < x) {
 
            apply_chmin(p, x);
            return;
        }
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        range_chmin(
            p << 1,
            l,
            m,
            ql,
            qr,
            x
        );
 
        range_chmin(
            p << 1 | 1,
            m + 1,
            r,
            ql,
            qr,
            x
        );
 
        puint(p);
    }
 
    // ============================================================
    // RANGE CHMAX
    // ============================================================
 
    void range_chmax(
        int p,
        int l,
        int r,
        int ql,
        int qr,
        int x
    ) {
        if (r < ql || qr < l || st[p].mn1 >= x)
            return;
 
        if (ql <= l &&
            r <= qr &&
            st[p].mn2 > x) {
 
            apply_chmax(p, x);
            return;
        }
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        range_chmax(
            p << 1,
            l,
            m,
            ql,
            qr,
            x
        );
 
        range_chmax(
            p << 1 | 1,
            m + 1,
            r,
            ql,
            qr,
            x
        );
 
        puint(p);
    }
 
    // ============================================================
    // RANGE SET
    // ============================================================
 
    void range_set(
        int p,
        int l,
        int r,
        int ql,
        int qr,
        int x
    ) {
        if (r < ql || qr < l)
            return;
 
        if (ql <= l && r <= qr) {
            apply_set(p, l, r, x);
            return;
        }
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        range_set(
            p << 1,
            l,
            m,
            ql,
            qr,
            x
        );
 
        range_set(
            p << 1 | 1,
            m + 1,
            r,
            ql,
            qr,
            x
        );
 
        puint(p);
    }
 
    // ============================================================
    // RANGE ADD
    // ============================================================
 
    void range_add(
        int p,
        int l,
        int r,
        int ql,
        int qr,
        int x
    ) {
        if (r < ql || qr < l)
            return;
 
        if (ql <= l && r <= qr) {
            apply_add(p, l, r, x);
            return;
        }
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        range_add(
            p << 1,
            l,
            m,
            ql,
            qr,
            x
        );
 
        range_add(
            p << 1 | 1,
            m + 1,
            r,
            ql,
            qr,
            x
        );
 
        puint(p);
    }
 
    // ============================================================
    // RANGE MOD
    //
    // a[i] = a[i] % x
    // ============================================================
 
    void range_mod(
        int p,
        int l,
        int r,
        int ql,
        int qr,
        int x
    ) {
        if (r < ql || qr < l)
            return;
 
        // --------------------------------------------------------
        // IMPORTANT
        //
        // For non-negative values and x > 0:
        //
        //     mx1 < x
        //
        // means modulo changes nothing.
        //
        // For signed values this condition alone is NOT enough.
        // We therefore only use this optimization when mn1 >= 0.
        // --------------------------------------------------------
 
        if (st[p].mn1 >= 0 && st[p].mx1 < x)
            return;
 
        // Leaf
        if (l == r) {
            apply_mod(p, x);
            return;
        }
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        range_mod(
            p << 1,
            l,
            m,
            ql,
            qr,
            x
        );
 
        range_mod(
            p << 1 | 1,
            m + 1,
            r,
            ql,
            qr,
            x
        );
 
        puint(p);
    }
 
    // ============================================================
    // RANGE SUM
    // ============================================================
 
    int query_sum(
        int p,
        int l,
        int r,
        int ql,
        int qr
    ) {
        if (r < ql || qr < l)
            return 0;
 
        if (ql <= l && r <= qr)
            return st[p].sum;
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        return query_sum(
                   p << 1,
                   l,
                   m,
                   ql,
                   qr
               )
             +
               query_sum(
                   p << 1 | 1,
                   m + 1,
                   r,
                   ql,
                   qr
               );
    }
 
    // ============================================================
    // RANGE MIN
    // ============================================================
 
    int query_min(
        int p,
        int l,
        int r,
        int ql,
        int qr
    ) {
        if (r < ql || qr < l)
            return INF;
 
        if (ql <= l && r <= qr)
            return st[p].mn1;
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        return min(
            query_min(p << 1, l, m, ql, qr),
            query_min(p << 1 | 1, m + 1, r, ql, qr)
        );
    }
 
    // ============================================================
    // RANGE MAX
    // ============================================================
 
    int query_max(
        int p,
        int l,
        int r,
        int ql,
        int qr
    ) {
        if (r < ql || qr < l)
            return -INF;
 
        if (ql <= l && r <= qr)
            return st[p].mx1;
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        return max(
            query_max(p << 1, l, m, ql, qr),
            query_max(p << 1 | 1, m + 1, r, ql, qr)
        );
    }
 
    // ============================================================
    // POINT SET
    // ============================================================
 
    void point_set(
        int p,
        int l,
        int r,
        int pos,
        int x
    ) {
        if (l == r) {
            apply_set(p, l, r, x);
            return;
        }
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        if (pos <= m) {
            point_set(
                p << 1,
                l,
                m,
                pos,
                x
            );
        }
        else {
            point_set(
                p << 1 | 1,
                m + 1,
                r,
                pos,
                x
            );
        }
 
        puint(p);
    }
 
    // ============================================================
    // POINT QUERY
    // ============================================================
 
    int point_query(
        int p,
        int l,
        int r,
        int pos
    ) {
        if (l == r)
            return st[p].sum;
 
        push(p, l, r);
 
        int m = (l + r) >> 1;
 
        if (pos <= m)
            return point_query(
                p << 1,
                l,
                m,
                pos
            );
 
        return point_query(
            p << 1 | 1,
            m + 1,
            r,
            pos
        );
    }
 
    // ============================================================
    // PUBLIC API
    // ============================================================
 
    void chmin(int l, int r, int x) {
        range_chmin(1, 0, n - 1, l, r, x);
    }
 
    void chmax(int l, int r, int x) {
        range_chmax(1, 0, n - 1, l, r, x);
    }
 
    void assign(int l, int r, int x) {
        range_set(1, 0, n - 1, l, r, x);
    }
 
    void add(int l, int r, int x) {
        range_add(1, 0, n - 1, l, r, x);
    }
 
    void mod(int l, int r, int x) {
        range_mod(1, 0, n - 1, l, r, x);
    }
 
    int sum(int l, int r) {
        return query_sum(1, 0, n - 1, l, r);
    }
 
    int minimum(int l, int r) {
        return query_min(1, 0, n - 1, l, r);
    }
 
    int maximum(int l, int r) {
        return query_max(1, 0, n - 1, l, r);
    }
 
    void set_point(int pos, int x) {
        point_set(1, 0, n - 1, pos, x);
    }
 
    int get_point(int pos) {
        return point_query(1, 0, n - 1, pos);
    }
};
