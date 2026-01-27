template <typename T, bool one_based = true>
struct FenwickRange {
    int n;
    vector<T> bit1, bit2;

    FenwickRange(int n) {
        this->n = n + 1;
        bit1.assign(n + 1, T());
        bit2.assign(n + 1, T());
    }

    // internal point update
    void update(vector<T>& bit, int idx, T val) {
        for (idx += (!one_based); idx < n; idx += idx & -idx)
            bit[idx] += val;
    }

    // internal prefix sum
    T query(const vector<T>& bit, int idx) const {
        T res = T();
        for (idx += (!one_based); idx > 0; idx -= idx & -idx)
            res += bit[idx];
        return res;
    }

    // add val to range [l, r]
    void update_range(int l, int r, T val) {
        update(bit1, l, val);
        update(bit1, r + 1, -val);

        update(bit2, l, val * (l - one_based));
        update(bit2, r + 1, -val * r);
    }

    // prefix sum [1 .. idx]
    T prefix_sum(int idx) const {
        return query(bit1, idx) * idx - query(bit2, idx);
    }

    // range sum [l .. r]
    T range_query(int l, int r) const {
        if (l > r) return T();
        return prefix_sum(r) - prefix_sum(l - 1);
    }
};
