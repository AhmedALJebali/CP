// Internal Edge representation
struct Edge {
    int u, v;
    long long w;
};

// Internal Disjoint Set Union
struct DSU {
    int n;
    vector<int> parent, sz;
    DSU(int n) : n(n), parent(n), sz(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int v) {
        return v == parent[v] ? v : parent[v] = find(parent[v]); // Path compression
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            // Union by size
            if (sz[a] < sz[b]) swap(a, b);
            parent[b] = a;
            sz[a] += sz[b];
            return true;
        }
        return false;
    }
};

struct BoruvkaMST {
    int n;
    vector<Edge> edges;

    // Initialize with n nodes (0-indexed: 0 to n-1)
    BoruvkaMST(int n) : n(n) {}

    // Add a bi-directional edge to the graph
    void add_edge(int u, int v, long long w) {
        edges.push_back({u, v, w});
    }

    // Calculates and returns the MST weight. Returns -1 if the graph is disconnected.
    long long get_mst() {
        DSU dsu(n);
        long long mst_weight = 0;
        int components = n;

        // Boruvka runs in at most O(log V) phases
        while (components > 1) {
            // Store the index of the cheapest edge for each component
            vector<int> cheapest(n, -1);
            bool any_merged = false;

            // Phase 1: Find the absolute cheapest outgoing edge for every component
            for (int i = 0; i < edges.size(); i++) {
                int comp_u = dsu.find(edges[i].u);
                int comp_v = dsu.find(edges[i].v);

                if (comp_u != comp_v) { // If they don't already belong to the same component
                    if (cheapest[comp_u] == -1 || edges[i].w < edges[cheapest[comp_u]].w) {
                        cheapest[comp_u] = i;
                    }
                    if (cheapest[comp_v] == -1 || edges[i].w < edges[cheapest[comp_v]].w) {
                        cheapest[comp_v] = i;
                    }
                }
            }

            // Phase 2: Add those cheapest edges simultaneously
            for (int i = 0; i < n; i++) {
                if (cheapest[i] != -1) {
                    int u = edges[cheapest[i]].u;
                    int v = edges[cheapest[i]].v;
                    long long w = edges[cheapest[i]].w;

                    // Unite the components. If successful, add weight.
                    if (dsu.unite(u, v)) {
                        mst_weight += w;
                        components--;
                        any_merged = true;
                    }
                }
            }

            // If we completed a phase and couldn't merge anything, it's disconnected
            if (!any_merged) break;
        }

        return components == 1 ? mst_weight : -1;
    }
};