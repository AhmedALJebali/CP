
struct Node {
    int v;
    Node() { v = 0; } 
    Node(int x) { this->v = x; } 
    Node operator +(const Node &other) const {
        Node res;
        // std::gcd (C++17) is the standard alternative to the GNU extension __gcd
        res.v = std::gcd(v, other.v); 
        return res;
    }
};

struct segTree_2d {
private: 
    vector<vector<Node>> t;
    int n, m;

public: 
    // Initializes the dimensions without needing power-of-2 padding
    void init(int n, int m) {
        this->n = n;
        this->m = m;
        t.assign(2 * n, vector<Node>(2 * m));    
    }

    void build(const vector<vector<int>> &g) {
        // 1. Initialize the leaves (bottom-right quadrant of the 2D array)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                t[i + n][j + m] = Node(g[i][j]);
            }
        }
        // 2. Build the bottom rows horizontally (y-axis for each leaf x)
        for (int i = 0; i < n; ++i) {
            for (int j = m - 1; j > 0; --j) {
                t[i + n][j] = t[i + n][j << 1] + t[i + n][j << 1 | 1];
            }
        }
        // 3. Build the upper rows vertically (x-axis for all y)
        for (int i = n - 1; i > 0; --i) {
            for (int j = 1; j < 2 * m; ++j) {
                t[i][j] = t[i << 1][j] + t[i << 1 | 1][j];
            }
        }
    }

    void upd(int r, int c, int val) {
        // Update the specific leaf
        t[r + n][c + m] = Node(val);
        
        // Update the y-axis (horizontal) for the specific x leaf
        for (int j = (c + m) >> 1; j > 0; j >>= 1) {
            t[r + n][j] = t[r + n][j << 1] + t[r + n][j << 1 | 1];
        }
        
        // Update the upper rows (x-axis)
        for (int i = (r + n) >> 1; i > 0; i >>= 1) {
            // Recalculate the specific y-column from vertical children
            t[i][c + m] = t[i << 1][c + m] + t[i << 1 | 1][c + m];
            
            // Recalculate horizontal parents for this newly updated node
            for (int j = (c + m) >> 1; j > 0; j >>= 1) {
                t[i][j] = t[i][j << 1] + t[i][j << 1 | 1];
            }
        }
    }

    // Queries the subgrid from (x1, y1) to (x2, y2) inclusive
    int qry(int x1, int y1, int x2, int y2) {
        Node res;
        // Loop over the X-axis bounds
        for (int r1 = x1 + n, r2 = x2 + n + 1; r1 < r2; r1 >>= 1, r2 >>= 1) {
            if (r1 & 1) {
                // If the left X-bound is odd, process its Y-axis range
                for (int c1 = y1 + m, c2 = y2 + m + 1; c1 < c2; c1 >>= 1, c2 >>= 1) {
                    if (c1 & 1) res = res + t[r1][c1++];
                    if (c2 & 1) res = res + t[r1][--c2];
                }
                r1++;
            }
            if (r2 & 1) {
                --r2;
                // If the right X-bound is odd, process its Y-axis range
                for (int c1 = y1 + m, c2 = y2 + m + 1; c1 < c2; c1 >>= 1, c2 >>= 1) {
                    if (c1 & 1) res = res + t[r2][c1++];
                    if (c2 & 1) res = res + t[r2][--c2];
                }
            }
        }
        return res.v;
    }
};
