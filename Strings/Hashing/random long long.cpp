static std::mt19937_64 RNG(
    (unsigned) chrono::steady_clock::now().time_since_epoch().count()
);

int rnd(int l, int r) {
    std::uniform_int_distribution<int> dist(l, r);
    return dist(RNG);
}
const int MOD = 1e9 + 9;
const int base = 67;
int modPow(int a, int e, int m) {
  int res = 1 % m;
  a %= m;
  while (e) {
    if (e & 1) res = (res * a) % m;
    a = (a * a) % m;
    e >>= 1;
  }
  return res;
}

int modInv2(int a, int m) {
  // works ONLY if m is prime and gcd(a, m) = 1
  return modPow(a, m - 2, m);
}