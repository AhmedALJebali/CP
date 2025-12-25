       vector<long long> dis(n, INF);
        dis[s] = 0;

        // Bellman-Ford: shortest paths
        for (int i = 0; i < n - 1; i++) {
            for (auto &[u, v, w]: edges) {
                if (dis[u] != INF && dis[u] + w < dis[v]) {
                    dis[v] = dis[u] + w;
                }
            }
        }

        // Detect & propagate negative cycles
        for (int i = 0; i < n; i++) {
            for (auto &[u, v, w]: edges) {
                if (dis[u] != INF && dis[u] + w < dis[v]) {
                    dis[v] = -INF;
                }
            }
        }
