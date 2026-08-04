int get_not_coprimes(int x, vector<int> &mul) {
  vector<int> p;
  while (x > 1) {
    int prime = spf[x];
    p.push_back(prime);
    while (x % prime == 0) {
      x /= prime;
    }
  }
  int nc = 0;
  int msk = 1LL << p.size();
  for (int j = 1; j < msk; j++) {
    int lcm = 1;
    for (int idx = 0; idx < p.size(); idx++) {
      if (j >> idx & 1) lcm *= p[idx];
    }
    if (__builtin_popcountll(j) & 1) nc += mul[lcm];
    else nc -= mul[lcm];
  }
  return nc;
}

int countSubseqGcdOne(const vector<int>& a) {
  map<int, int> dp;
  for (int x : a) {
    map<int, int> ndp = dp;
    ndp[x]++;
    for (auto [g, cnt] : dp)
      ndp[gcd(g, x)] = (ndp[gcd(g, x)]+cnt)%MOD;
    dp.swap(ndp);
  }
  return dp[1]%MOD;
}

int countSubseqGcdOne(vector<int>& a) {
  int n=a.size();
  int MAXA = *max_element(all(a));
  vector<int> freq(MAXA + 1, 0);
  for (int v : a)
    ++freq[v];
  // ---------- Linear sieve for Möbius ----------
  vector<int> mu(MAXA + 1);
  vector<int> lp(MAXA + 1, 0);
  vector<int> primes;
  mu[1] = 1;
  for (int i = 2; i <= MAXA; i++) {
    if (lp[i] == 0) {
      lp[i] = i;
      primes.push_back(i);
      mu[i] = -1;
    }
    for (int p : primes) {
      int v = 1LL * p * i;
      if (v > MAXA) break;
      lp[v] = p;
      if (p == lp[i]) {
        mu[v] = 0;
        break;
      } else {
        mu[v] = -mu[i];
      }
    }
  }
  // ---------- Count multiples ----------
  vector<int> cnt(MAXA + 1, 0);
  for (int d = 1; d <= MAXA; d++) {
    for (int m = d; m <= MAXA; m += d)
      cnt[d] += freq[m];
  }
  // ---------- Powers of two ----------
  vector<int> pw(n + 1);
  pw[0] = 1;
  for (int i = 1; i <= n; i++)
    pw[i] = (pw[i - 1] * 2LL) % MOD;
  int ans = 0;
  for (int d = 1; d <= MAXA; d++) {
    if (mu[d] == 0) continue;
    int ways = pw[cnt[d]] - 1;
    if (ways < 0) ways += MOD;
    ans += 1LL * mu[d] * ways;
    ans %= MOD;
  }
  ans %= MOD;
  if (ans < 0) ans += MOD;
  return ans;
}

