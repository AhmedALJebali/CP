int ExtendedEuclid(int a, int b, int &x, int &y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  int x1, y1;
  int g = ExtendedEuclid(b, a % b, x1, y1);
  x = y1;
  y = x1 - (a / b) * y1;
  return g;
}

int modInv(int a, int m) {
  int x0, y0;
  int g = ExtendedEuclid(a, m, x0, y0);
  return (x0 % m + m) % m;
}
// Fermat’s Little Theorem (FLT)
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