class PrefixSum2D {
  vector<vector<int>> prefix;
public:
  PrefixSum2D(const vector<vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return;
    int n = matrix.size();
    int m = matrix[0].size();
    prefix.assign(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        prefix[i + 1][j + 1] = matrix[i][j]
                             + prefix[i][j + 1]
                             + prefix[i + 1][j]
                             - prefix[i][j];
      }
    }
  }
  int query(int row1, int col1, int row2, int col2) {
    if (prefix.empty()) return 0;
    int r1 = row1, c1 = col1;
    int r2 = row2 + 1, c2 = col2 + 1;
    return prefix[r2][c2] - prefix[r1][c2] - prefix[r2][c1] + prefix[r1][c1];
  }
};
