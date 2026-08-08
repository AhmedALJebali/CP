const int N=3e3+5;
const int INF = 2e18;
 
int dp[N],prv[N],pref[N];
 
int cost (int l,int r) {
  int sum=pref[r]-(l?pref[l-1]:0);
  return sum*sum;
}
 
void dq(int l,int r,int optl,int optr) {
  if (l > r) return ;
  int mid = l + (r - l) / 2;
  int opt = optl;
  dp[mid] = INF;
  for (int i = optl; i <= min(mid, optr); i++) {
    int c = cost(i, mid);
    if ((prv[i-1] + c) < dp[mid]) {
      dp[mid] = prv[i-1] + c;
      opt = i;
    }
  }
  dq(l, mid - 1, optl, opt);
  dq(mid + 1, r, opt, optr);
}
void solve(){
  int n,k;
  cin>>n>>k;
  vector<int>a(n);
  for (int i=0;i<n;i++) {
    cin>>a[i];
    pref[i]=a[i];
    if (i) pref[i]+=pref[i-1];
  }
  for (int i = 0; i < n; i++) {
    prv[i] = cost(0, i);
  }
  for (int i = 1; i < k; i++) {
    dq(0, n-1, 1, n-1);
    for(int j = 0; j < n; j++) {
      prv[j] = dp[j];
    }
  }
  cout << prv[n-1] << "\n";
}
 

