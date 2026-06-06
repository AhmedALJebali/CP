template<class T = long long>
struct Prim {
    struct Edge {
        int to;
        T w;
    };

    int n;
    vector<vector<Edge>> adj;

    // Initialize with the number of nodes (0-indexed)
    void init(int _n) {
        n = _n;
        adj.assign(n, vector<Edge>());
    }

    // Add an undirected edge
    void add_edge(int u, int v, T w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // Returns {total_weight, vector_of_mst_edges}
    // Returns {-1, {}} if the graph is disconnected
    pair<T, vector<pair<int, int>>> solve(int start_node = 0) {
        T total_weight = 0;
        vector<bool> visited(n, false);
        vector<pair<int, int>> mst_edges;
        
        // Min-heap: stores {weight, {current_node, parent_node}}
        using pq_element = pair<T, pair<int, int>>;
        priority_queue<pq_element, vector<pq_element>, greater<pq_element>> pq;

        // Push starting node with 0 weight and no parent (-1)
        pq.push({0, {start_node, -1}});

        int nodes_connected = 0;

        while (!pq.empty() && nodes_connected < n) {
            auto [w, nodes] = pq.top();
            auto [u, parent] = nodes;
            pq.pop();

            // If we already visited this node, skip it
            if (visited[u]) continue;

            // Mark visited and add to our MST
            visited[u] = true;
            total_weight += w;
            nodes_connected++;
            
            if (parent != -1) {
                mst_edges.push_back({parent, u});
            }

            // Push all unvisited neighbors into the priority queue
            for (auto& edge : adj[u]) {
                if (!visited[edge.to]) {
                    pq.push({edge.w, {edge.to, u}});
                }
            }
        }

        // If we didn't connect all nodes, no valid MST exists
        if (nodes_connected < n) return {-1, {}};
        
        return {total_weight, mst_edges};
    }
};