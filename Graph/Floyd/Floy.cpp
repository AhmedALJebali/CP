void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<int> > dp(n, vector<int>(n, oo));
    for (int i = 0; i < n; i++) dp[i][i] = 0;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        dp[u][v] = min(dp[u][v], w);
        dp[v][u] = min(dp[v][u], w);
    }
    for (int k = 0; k < n; k++) {
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                dp[u][v] = min(dp[u][v], dp[u][k] + dp[k][v]);
            }
        }
    }
    while (q--) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        if (dp[u][v] >= oo) {
            cout << -1 << endl;
            continue;
        }
        cout << dp[u][v] << endl;
    }
}
 
