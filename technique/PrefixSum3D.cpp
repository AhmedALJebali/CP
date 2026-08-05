
class PrefixSum3D {
private:
  vector<vector<vector<int>>> prefix;
public:
  PrefixSum3D(const vector<vector<vector<int>>>& grid) {
    if (grid.empty() || grid[0].empty() || grid[0][0].empty()) return;
    int n = grid.size();
    int m = grid[0].size();
    int p = grid[0][0].size();
    prefix.assign(n + 1, vector<vector<int>>(m + 1, vector<int>(p + 1, 0)));
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= m; j++) {
        for (int k = 1; k <= p; k++) {
          prefix[i][j][k] = grid[i-1][j-1][k-1]
                          + prefix[i-1][j][k]
                          + prefix[i][j-1][k]
                          + prefix[i][j][k-1]
                          - prefix[i-1][j-1][k]
                          - prefix[i-1][j][k-1]
                          - prefix[i][j-1][k-1]
                          + prefix[i-1][j-1][k-1];
        }
      }
    }
  }
  int query(int x1, int y1, int z1, int x2, int y2, int z2) {
    if (prefix.empty()) return 0;
    int X1 = x1,     Y1 = y1,     Z1 = z1;
    int X2 = x2 + 1, Y2 = y2 + 1, Z2 = z2 + 1;

    return prefix[X2][Y2][Z2] 
         - prefix[X1][Y2][Z2] 
         - prefix[X2][Y1][Z2] 
         - prefix[X2][Y2][Z1] 
         + prefix[X1][Y1][Z2] 
         + prefix[X1][Y2][Z1] 
         + prefix[X2][Y1][Z1] 
         - prefix[X1][Y1][Z1];
  }
};
