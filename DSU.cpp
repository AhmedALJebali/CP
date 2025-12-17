struct DSU {
    vector<int> parent, sz, mne, mxe;
    int comp, mxcomp;

    DSU(int n) {
        parent.resize(n + 1);
        mne.resize(n + 1);
        mxe.resize(n + 1);
        sz.assign(n + 1, 1);
        comp = n;
        mxcomp = 1;
        iota(parent.begin(), parent.end(), 0);
        iota(mne.begin(), mne.end(), 0);
        iota(mxe.begin(), mxe.end(), 0);
    }
 
    int find_root(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find_root(parent[x]);
    }
 
    bool unite(int x, int y) {
        int root_x = find_root(x), root_y = find_root(y);
        if (root_x == root_y) return false;
        if (sz[root_x] < sz[root_y])swap(root_x, root_y);
        sz[root_x] += sz[root_y];
        mne[root_x] = min(mne[root_x], mne[root_y]);
        mxe[root_x] = max(mxe[root_x], mxe[root_y]);
        parent[root_y] = root_x;
        mxcomp = max(mxcomp, sz[root_x]);
        comp--;
        return true;
    }

    bool same(int x, int y) {
        return find_root(x) == find_root(y);
    }
 
    int siz(int x) {
        return sz[find_root(x)];
    }
};
vector<int> nxt(n + 2);

for (int i = 1; i <= n + 1; i++)
    nxt[i] = i;

int get(int x) {
    if (nxt[x] == x) return x;
    return nxt[x] = get(nxt[x]);
}
 if (res[p] == v[p]) {
                    nxt[p] = get(p + 1);
 }
