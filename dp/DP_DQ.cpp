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
    if ((prv[i-1] + c) <= dp[mid]) {
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
 const int N = 35000 + 5;

int dp[N], prv[N],c[N],frq[N];
int cur=0,L = 1, R = 0;
void add(int idx) {
  if (++frq[c[idx]] == 1) cur++;
}
void remove(int idx) {
  if (--frq[c[idx]] == 0) cur--;
}
 int qry(int l, int r) {
  while (L > l) add(--L);
  while (R < r) add(++R);
  while (L < l) remove(L++);
  while (R > r) remove(R--);
  return cur;
}
void dq(int l, int r, int optl, int optr) {
  if (l > r) return;
  int mid = l + (r - l) / 2;
  int opt = optl;
  dp[mid] = 0;
  for (int i = optl; i <= min(mid, optr); i++) {
    int cost = qry(i,mid);
    if (prv[i - 1] + cost >= dp[mid]) {
      dp[mid] = prv[i - 1] + cost;
      opt = i;
    }
  }
  dq(l, mid - 1, optl, opt);
  dq(mid + 1, r, opt, optr);
}



const int N = 35000+ 5;

int dp[N], prv[N],c[N];
int cur=0,L = 1, R = 0;
deque<int>f[N];

void add(int idx,int dir) {
  if (!f[c[idx]].empty()) {
    cur -= f[c[idx]].back()- f[c[idx]].front();
  }
  if (dir==2)
    f[c[idx]].push_back(idx);
  else
    f[c[idx]].push_front(idx);

  cur += f[c[idx]].back()- f[c[idx]].front();
}
void remove(int idx,int dir) {
  cur -= f[c[idx]].back()- f[c[idx]].front();

  if (dir==2)
    f[c[idx]].pop_back();
  else
    f[c[idx]].pop_front();

  if (!f[c[idx]].empty()) {
    cur += f[c[idx]].back()- f[c[idx]].front();
  }
}
 int qry(int l, int r) {
  while (L > l) add(--L,1);
  while (R < r) add(++R,2);
  while (L < l) remove(L++,1);
  while (R > r) remove(R--,2);
  return cur;
}
void dq(int l, int r, int optl, int optr) {
  if (l > r) return;
  int mid = l + (r - l) / 2;
  int opt = optl;
  dp[mid] = 1e9;
  for (int i = optl; i <= min(mid, optr); i++) {
    int cost = qry(i,mid);
    if (prv[i - 1] + cost <= dp[mid]) {
      dp[mid] = prv[i - 1] + cost;
      opt = i;
    }
  }
  dq(l, mid - 1, optl, opt);
  dq(mid + 1, r, opt, optr);
}

