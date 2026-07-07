#include <bits/stdc++.h>
using namespace std;
#define int long long
#define ld long double
#define all(v) v.begin(), v.end()
#define FAST                    \
ios_base::sync_with_stdio(0); \
cin.tie(nullptr);             \
cout.tie(nullptr);
#define fileIO                      \
freopen("input.txt", "r", stdin); \
freopen("out.txt", "w", stdout)
const int MOD=1e8;
const int INF=2e18;
/* ========================================================================
FUNCTION: getdp(n, k)
Generates the number of ways to partition 'n' soldiers into 'j' groups, 
where no group exceeds size 'k'.

THE DP STATE EXPLAINED:
-----------------------
Normally, dp[i][j] = ways to place the REMAINING (n - i) soldiers into exactly 'j' groups.
(i.e., 'i' soldiers have already been placed).

Because we want to optimize this from O(N^3) to O(N^2), the array `dp` serves a dual purpose:
1. First, it calculates the raw answer for dp[i][j].
2. Then, it immediately turns itself into a PREFIX SUM array so the next column (j+1) can use it in O(1) time.
========================================================================
*/
vector<vector<int>> getdp(int n, int k) {
  // dp[i][j] will store the prefix sum of ways to partition elements.
  vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
  
  // BASE CASE: 
  // If we have already placed all 'n' soldiers, and we need to form '0' more groups, 
  // there is exactly 1 valid way to do this (we just stop).
  dp[n][0] = 1;
  
  // Convert column 0 into a prefix sum array.
  // Before this loop: dp = [0, 0, ..., 0, 1]
  // After this loop:  dp = [0, 0, ..., 0, 1] (since only dp[n] has value 1, prefix sum holds 1 only at n).
  for (int i = 1; i <= n; i++) {
    dp[i][0] = (dp[i][0] + dp[i - 1][0]) % MOD;
  }

  // Iterate over the number of groups we want to form (from 1 up to n)
  for (int j = 1; j <= n; j++) {
      
    // TRANSITION STEP (Calculating the raw DP state):
    // We iterate backwards because this is a Suffix DP (we look forward at i+x).
    for (int i = n - 1; i >= 0; --i) {
        // THE TRANSITION MATH:
        // We want to form a new group of size 'x' (where x is between 1 and k).
        // This means we need the sum of the previous column (j-1) from index (i + 1) to min(n, i + k).
        // Range Sum Formula: Sum[L to R] = Prefix[R] - Prefix[L - 1]
        // Here: 
        // L = i + 1  ---> L - 1 = i
        // R = min(n, i + k)
        // So the sum is simply: Prefix[R] - Prefix[i].
        dp[i][j] = (dp[min(n, i + k)][j - 1] - dp[i][j - 1] + MOD) % MOD;
    }
    
    // PREFIX SUM STEP (Preparing the state for the next group iteration j+1):
    // We overwrite the raw answers with their running totals (accumulative sum)
    // so the next iteration can use the Range Sum Formula in O(1) time.
    for (int i = 1; i <= n; i++) {
      dp[i][j] = (dp[i][j] + dp[i - 1][j]) % MOD;
    }
  }
  
  return dp;
}

void solve() {
    int n1, n2, k1, k2;
    cin >> n1 >> n2 >> k1 >> k2;
    
    // getdp()[0] gets the 0-th row of the DP table.
    // dp[0][g] represents: "0 soldiers placed so far, how many ways to place ALL 'n' soldiers into 'g' groups?"
    auto dp1 = getdp(n1, k1)[0];
    auto dp2 = getdp(n2, k2)[0];
    
    int ans = 0;
    int mx = max(n1, n2);
    
    // COMBINATORICS MERGE:
    // We try every possible total number of groups 'g'.
    // To ensure footmen and horsemen alternate (no two groups of the same type touch),
    // the number of groups for each type must either be EXACTLY EQUAL, or differ by EXACTLY 1.
    for (int g = 1; g <= mx; g++) {
        
      // Case 1: Equal groups (g footmen groups, g horsemen groups)
      // Alternating patterns: F-H-F-H or H-F-H-F
      // Since there are 2 valid starting patterns, we multiply the combinations by 2.
      if (g <= n1 && g <= n2) {
        ans = (ans + (dp1[g] * dp2[g]) % MOD * 2) % MOD;
      }
      
      // Case 2: Footmen have 1 more group (g+1 footmen, g horsemen)
      // Alternating pattern: F-H-F-H-F
      // Forced to start and end with Footmen (only 1 valid pattern).
      if (g + 1 <= n1 && g <= n2) {
        ans = (ans + (dp1[g + 1] * dp2[g]) % MOD) % MOD;
      }
      
      // Case 3: Horsemen have 1 more group (g footmen, g+1 horsemen)
      // Alternating pattern: H-F-H-F-H
      // Forced to start and end with Horsemen (only 1 valid pattern).
      if (g <= n1 && g + 1 <= n2) {
        ans = (ans + (dp1[g] * dp2[g + 1]) % MOD) % MOD;
      }
    }
    
    cout << ans << "\n";
}
signed main(){
  FAST;
#ifndef ONLINE_JUDGE
  fileIO;
#endif
  int t = 1;
  // cin >> t;
  for (int i = 1; i <= t; i++) {
    solve();
  }
  return 0;
}
