int SP[LOGN][LOGM][N][M];

// Merge function
int merge(int a, int b) {
    return a + b; // sum
}

// -------------------- Build --------------------
void build(vector<vector<int>> &a) {
    int n = a.size();
    int m = a[0].size();

    // Base case: 1x1
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            SP[0][0][i][j] = a[i][j];

    for (int k1 = 0; (1 << k1) <= n; k1++) {
        for (int k2 = 0; (1 << k2) <= m; k2++) {
            if (k1 == 0 && k2 == 0) continue;

            for (int i = 0; i + (1 << k1) <= n; i++) {
                for (int j = 0; j + (1 << k2) <= m; j++) {
                    if (k1 > 0 && k2 > 0) {
                        int x = merge(SP[k1-1][k2-1][i][j],
                                      SP[k1-1][k2-1][i+(1<<(k1-1))][j]);
                        int y = merge(SP[k1-1][k2-1][i][j+(1<<(k2-1))],
                                      SP[k1-1][k2-1][i+(1<<(k1-1))][j+(1<<(k2-1))]);
                        SP[k1][k2][i][j] = merge(x, y);
                    } else if (k1 > 0) {
                        SP[k1][k2][i][j] = merge(SP[k1-1][k2][i][j],
                                                 SP[k1-1][k2][i+(1<<(k1-1))][j]);
                    } else { // k2 > 0
                        SP[k1][k2][i][j] = merge(SP[k1][k2-1][i][j],
                                                 SP[k1][k2-1][i][j+(1<<(k2-1))]);
                    }
                }
            }
        }
    }
}

// -------------------- Overlapping Query O(1) --------------------
int qry1(int x1, int y1, int x2, int y2) {
    int k1 = __lg(x2 - x1 + 1);
    int k2 = __lg(y2 - y1 + 1);

    int a = merge(SP[k1][k2][x1][y1], SP[k1][k2][x2-(1<<k1)+1][y1]);
    int b = merge(SP[k1][k2][x1][y2-(1<<k2)+1], SP[k1][k2][x2-(1<<k1)+1][y2-(1<<k2)+1]);

    return merge(a, b);
}

// -------------------- Non-Overlapping Query O(log n * log m) --------------------
int qry2(int x1, int y1, int x2, int y2) {
    int res = 0; // identity for sum
    for (int i = x1; i <= x2; ) {
        int k1 = __lg(x2 - i + 1);
        for (int j = y1; j <= y2; ) {
            int k2 = __lg(y2 - j + 1);
            res = merge(res, SP[k1][k2][i][j]);
            j += (1 << k2);
        }
        i += (1 << k1);
    }
    return res;
}
