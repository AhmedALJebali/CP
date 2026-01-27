template <typename T, bool one_based = true>
struct FenwickTree {
    vector<T> bit;
    int n;

    FenwickTree(int n) {
        this->n = n + 1;
        bit.assign(n + 1, T());
    }

    FenwickTree(const vector<T>& a) : FenwickTree(a.size()) {
        for (int i = 0; i < (int)a.size(); i++)
            update(i + one_based, a[i]);
    }

    // prefix query [1 .. idx]
    T query(int idx) const {
        T ret = T();
        for (idx += (!one_based); idx > 0; idx -= idx & -idx)
            ret += bit[idx];
        return ret;
    }

    // range query [l .. r]
    T query(int l, int r) const {
        if (l > r) return T();
        return query(r) - (l - 1 >= 0 ? query(l - 1) : T());
    }

    // point update
    void update(int idx, T delta) {
        for (idx += (!one_based); idx < n; idx += idx & -idx)
            bit[idx] += delta;
    }

    // range update (difference array trick)
    void update_range(int l, int r, T val) {
        update(l, val);
        update(r + 1, -val);
    }

    // first index where prefix sum >= x
    int lower_bound(T x) const {
        int idx = 0;
        int mask = 1;
        while (mask < n) mask <<= 1;

        for (mask >>= 1; mask > 0; mask >>= 1) {
            if (idx + mask < n && bit[idx + mask] < x) {
                x -= bit[idx + mask];
                idx += mask;
            }
        }
        return idx + one_based;
    }

    // first index where prefix sum > x
    int upper_bound(T x) const {
        int idx = 0;
        int mask = 1;
        while (mask < n) mask <<= 1;

        for (mask >>= 1; mask > 0; mask >>= 1) {
            if (idx + mask < n && bit[idx + mask] <= x) {
                x -= bit[idx + mask];
                idx += mask;
            }
        }
        return idx + one_based;
    }
};
