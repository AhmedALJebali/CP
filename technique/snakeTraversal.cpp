vector<int> snakeTraversal(const vector<vector<int>>& grid) {
    vector<int> result;
    if (grid.empty() || grid[0].empty()) return result;
    int n = grid.size();
    int m = grid[0].size();
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            // Even rows: Traverse left to right
            for (int j = 0; j < m; ++j) {
                result.push_back(grid[i][j]);
            }
        } else {
            // Odd rows: Traverse right to left
            for (int j = m - 1; j >= 0; --j) {
                result.push_back(grid[i][j]);
            }
        }
    }
    return result;
}
