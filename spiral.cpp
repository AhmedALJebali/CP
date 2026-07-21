// Spiral traversal starting at the center of the grid,
// moving in order: up, left, down, right, expanding step size as it goes.
vector<int> spiralFromCenter(vector<vector<int>>& grid) {
    vector<int> result;
    int n = grid.size();
    if (n == 0) return result;
    int m = grid[0].size();
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    int row = n / 2, col = m / 2;
    // direction vectors in order: up, left, down, right
    int dr[4] = {-1, 0, 1, 0};
    int dc[4] = {0, -1, 0, 1};
    // visit starting cell
    auto visit = [&](int r, int c) {
        if (r >= 0 && r < n && c >= 0 && c < m && !visited[r][c]) {
            visited[r][c] = true;
            result.push_back(grid[r][c]);
        }
    };
    visit(row, col);
    int totalCells = n * m;
    int step = 1;
    int dirIndex = 0; // 0=up, 1=left, 2=down, 3=right
    while ((int)result.size() < totalCells) {
        // each step length is used twice before increasing (standard spiral pattern)
        for (int rep = 0; rep < 2; rep++) {
            for (int s = 0; s < step; s++) {
                row += dr[dirIndex];
                col += dc[dirIndex];
                visit(row, col);
            }
            dirIndex = (dirIndex + 1) % 4;

            // safety check: if grid fully visited mid-loop, stop
            if ((int)result.size() >= totalCells) break;
        }
        step++;
    }
    return result;
}
