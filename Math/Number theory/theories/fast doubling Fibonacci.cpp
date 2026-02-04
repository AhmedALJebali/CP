// fast doubling Fibonacci
pair<int,int> fib(int n) {
  if (n == 0) return {0, 1};
  auto p = fib(n >> 1);
  int c = (1LL * p.first * ((2LL * p.second % MOD - p.first + MOD) % MOD)) % MOD;
  int d = (1LL * p.first * p.first % MOD + 1LL * p.second * p.second % MOD) % MOD;
  if (n & 1) return {d, (c + d) % MOD};
  return {c, d};
}
