int SQ = 2154;
const int N = 1e5 + 5;

struct Query {
    int l, r, t, idx, x, type;
    bool operator<(const Query &other) const {
        int b1_l = l / SQ;
        int b2_l = other.l / SQ;
        if (b1_l != b2_l) return b1_l < b2_l;

        int b1_r = r / SQ;
        int b2_r = other.r / SQ;
        if (b1_r != b2_r) {
            if (b1_l & 1) return b1_r < b2_r;
            return b1_r > b2_r;
        }

        // Sort by Time
        if (b1_r & 1) return t < other.t;
        return t > other.t;
    }
};

struct Update {
    int pos;
    int old_val;
    int new_val;
};
int Lpf[N];
vector<int> primes;
int f[N];
void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> arr(n + 1);
    vector<Query> qu;
    vector<Update> updates;
    vector<int> cur = arr;
    int query_idx = 0;
    for (int i = 0; i < q; i++) {
        int typ;
        cin >> typ;
        if (typ == 3) {
            int u, x;
            cin >> u >> x;
            int pos = in[u];
            updates.push_back({pos, cur[pos], x});
            cur[pos] = x;
        } else {
            int v, x = -1;
            cin >> v;
            if (typ == 2) cin >> x;
            qu.push_back({in[v], out[v], (int)updates.size(), query_idx++, x, typ});
        }
    }
    int dis = 0;
    auto add = [&](int idx, int type) {
        if(arr[idx] <= 1) return;
        if(Lpf[arr[idx]] != arr[idx]) return;
        dis += f[arr[idx]] == 0;
        f[arr[idx]]++;
    };
    auto erase = [&](int idx, int type) {
        if(arr[idx] <= 1) return;
        if(Lpf[arr[idx]] != arr[idx]) return;
        f[arr[idx]]--;
        dis -= f[arr[idx]] == 0;
    };
    int L = 1, R = 0, T = 0;
    auto apply_update = [&](int t_idx, int dir) {
        int p = updates[t_idx].pos;
        bool in_window = (L <= p && p <= R);
        if (in_window) {
            erase(p, 1);
        }
        if (dir == 1) {
            arr[p] = updates[t_idx].new_val;
        } else {
            arr[p] = updates[t_idx].old_val;
        }
        if (in_window) {
            add(p, 1);
        }
    };
    sort(qu.begin(), qu.end());
    vector<int> ans(qu.size());
    for (auto &[l, r, t, idx, x, type] : qu) {
        while (T < t) apply_update(T++, 1);
        while (T > t) apply_update(--T, -1);
        while (R < r) add(++R, 1);
        while (L > l) add(--L, 2);
        while (R > r) erase(R--, 1);
        while (L < l) erase(L++, 2);
        ans[idx] = (type == 2 ? f[x]!=0 : dis);
    }
    for (int i = 0; i < qu.size(); i++) {
        cout << ans[i] << "\n";
    }
}
