struct SquareSPA {
    int n, m, K;
    vector<int> t;

    // Fast inline merge function utilizing built-in hardware instructions
    inline int merge(int a, int b) const { return std::gcd(a, b); }

    // 1D flat index for a 3D table [k][i][j]
    inline int idx(int k, int i, int j) const {
        return k * n * m + i * m + j;
    }

    void build(const vector<vector<int>>& a) {
        n = a.size();
        m = a[0].size();

        // We only need powers of 2 up to the smallest dimension
        int min_dim = min(n, m);
        K = __lg(min_dim) + 1;

        // Allocate memory (Reduced from O(N*M*logN*logM) to O(N*M*log(min(N,M))))
        t.assign(K * n * m, 0);

        // Base case: 1x1 squares (k = 0)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                t[idx(0, i, j)] = a[i][j];
            }
        }

        // Build squares of size 2^k x 2^k
        for (int k = 1; k < K; k++) {
            int step = 1 << (k - 1);
            int cur_size = 1 << k;

            for (int i = 0; i + cur_size <= n; i++) {
                for (int j = 0; j + cur_size <= m; j++) {
                    // A 2^k square is made of four 2^(k-1) squares
                    int top_left  = t[idx(k - 1, i, j)];
                    int top_right = t[idx(k - 1, i, j + step)];
                    int bot_left  = t[idx(k - 1, i + step, j)];
                    int bot_right = t[idx(k - 1, i + step, j + step)];

                    t[idx(k, i, j)] = merge(merge(top_left, top_right), merge(bot_left, bot_right));
                }
            }
        }
    }

    // O(1) query for a SQUARE of size k x k starting at top-left (x, y)
    inline int querySquare(int x, int y, int k) const {
        if (k == 1) return t[idx(0, x, y)];

        int p = __lg(k);
        int step = 1 << p;

        // Cover the k x k square with four overlapping 2^p x 2^p squares
        int top_left  = t[idx(p, x, y)];
        int top_right = t[idx(p, x, y + k - step)];
        int bot_left  = t[idx(p, x + k - step, y)];
        int bot_right = t[idx(p, x + k - step, y + k - step)];

        return merge(merge(top_left, top_right), merge(bot_left, bot_right));
    }
};
