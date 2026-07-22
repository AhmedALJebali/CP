/*
 * Function to calculate the number of ways to get an EXACT sum.
 * n: number of dice
 * m: number of faces on each die
 * target: the exact sum we want to achieve
 */
int countWaysExact(int n, int m, int target) {
    // dp[i][j] = number of ways to get sum 'j' using 'i' dice
    vector<vector<int>> dp(n + 1, vector<int>(target + 1, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        vector<int> pref(target + 1, 0);
        pref[0] = dp[i-1][0];
        for (int j = 1; j <= target; j++) {
            pref[j] = (pref[j-1] + dp[i-1][j]) % MOD;
        }
        for (int j = 1; j <= target; j++) {
            int ways = pref[j - 1];
            if (j - m - 1 >= 0) {
                ways = (ways - pref[j - m - 1] + MOD) % MOD;
            }
            dp[i][j] = ways;
        }
    }
    return dp[n][target];
}
int countWaysExact2(int N, int M, int targetSum) {
    if (targetSum < N || targetSum > (long long)N * M) {
        return 0;
    }
    int total_ways = 0;
    int limit = (targetSum - N) / M;
    for (int k = 0; k <= limit; k++) {
        int part1 = nCr(N, k);
        int part2 = nCr(targetSum - k * M - 1, N - 1);
        int term = part1 * part2;
        if (k % 2 == 0) {
            total_ways += term; // Even k: Add to total
        } else {
            total_ways -= term; // Odd k: Subtract from total
        }
    }
    return total_ways;
}
int countWaysUpTo(int n, int m, int target) {
    int ret = 0;
    for (int i = n; i <= target; i++) {
        ret = (ret + countWaysExact(n, m, i)) % MOD;
    }
    return ret;
}
