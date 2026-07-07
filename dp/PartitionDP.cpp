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
vector<vector<int>> getdp(int n, int k) {
  vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
  dp[n][0] = 1;
  for (int i = 1; i <= n; i++) {
    dp[i][0] = (dp[i][0] + dp[i - 1][0]) % MOD;
  }
  for (int j = 1; j <= n; j++) {
    for (int i = n - 1; i >= 0; --i) {
      dp[i][j] = (dp[min(n, i + k)][j - 1] - dp[i][j - 1] + MOD) % MOD;
    }
    for (int i = 1; i <= n; i++) {
      dp[i][j] = (dp[i][j] + dp[i - 1][j]) % MOD;
    }
  }
  return dp;
}
void solve() {
    int n1, n2, k1, k2;
    cin >> n1 >> n2 >> k1 >> k2;
    auto dp1 = getdp(n1, k1)[0];
    auto dp2 = getdp(n2, k2)[0];
    int ans = 0;
    int mx = max(n1, n2);
    for (int g = 1; g <= mx; g++) {
      if (g <= n1 && g <= n2) {
        ans = (ans + (dp1[g] * dp2[g]) % MOD * 2) % MOD;
      }
      if (g + 1 <= n1 && g <= n2) {
        ans = (ans + (dp1[g + 1] * dp2[g]) % MOD) % MOD;
      }
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
