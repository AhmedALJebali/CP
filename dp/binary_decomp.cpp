void solve() {
  int n,x;cin>>n>>x;
  vector<int>w(n),v(n),k(n);
  for (int i=0;i<n;i++)cin>>w[i];
  for (int i=0;i<n;i++)cin>>v[i];
  for (int i=0;i<n;i++)cin>>k[i];
  vector<pair<int,int>> all;
  for (int i = 0; i < n; i++){
    int cnt = k[i];
    int base = 1;
    while(cnt > 0){
      int num = min(base, cnt);
      all.emplace_back(w[i] * num, v[i] * num);
      cnt -= num;
      base *= 2;
    }
  }
  vector<int>dp(x+1,0);
  for (int i=all.size()-1;i>=0;i--) {
    for (int rem=x;rem>=all[i].first;rem--) {
      dp[rem]=max(dp[rem-all[i].first]+all[i].second ,dp[rem]);
    }
  }
  cout<<*max_element(all(dp))<<endl;
}
