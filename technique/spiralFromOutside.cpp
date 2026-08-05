vector<int> spiralFromOutside(const vector<vector<int>>& grid) {
    vector<int> result;
    int n = grid.size();
    if (n == 0) return result;
    int m = grid[0].size();
    // Keep track of where we've been so we know when to turn
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    // Direction vectors for Clockwise: Right, Down, Left, Up
    int dr[] = {0, 1, 0, -1};
    int dc[] = {1, 0, -1, 0};
    int r = 0;
    int c = 0;
    int dir = 0;
    int totalCells = n * m;
    for (int i = 0; i < totalCells; ++i) {
        result.push_back(grid[r][c]);
        visited[r][c] = true;  
        // Calculate the next potential cell
        int nextR = r + dr[dir];
        int nextC = c + dc[dir];  
        // If the next cell is out of bounds OR already visited, we must turn
        if (nextR < 0 || nextR >= n || nextC < 0 || nextC >= m || visited[nextR][nextC]) {
            dir = (dir + 1) % 4; // Turn 90 degrees clockwise
            nextR = r + dr[dir];
            nextC = c + dc[dir];
        }   
        // Move to the next cell
        r = nextR;
        c = nextC;
    }
    return result;
}
