int gcd(int a, int b) {
  while (b) {
    a %= b;
    swap(a, b);
  }
  return a;
}

int gcd_rec(int a, int b) {
  return b ? gcd_rec(b, a % b) : a;
}

int lcm(int a,int b) {
  return (abs(a) / gcd(a, b)) * abs(b);
}


// Extended Euclidean Algorithm
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

int ceil_div(int a, int b) {
  if (b < 0) a = -a, b = -b;
  if (a >= 0) return (a + b - 1) / b;
  return a / b;
}

int floor_div(int a, int b) {
  if (b < 0) a = -a, b = -b;
  if (a >= 0) return a / b;
  return -((-a + b - 1) / b);
}

int modInv(int a, int m) {
  int x, y;
  int g = ExtendedEuclid(a, m, x, y);
  if (g != 1) return -1; // no inverse
  return (x % m + m) % m;
}
// fast doubling Fibonacci
pair<int,int> fib(int n) {
  if (n == 0) return {0, 1};
  auto p = fib(n >> 1);
  int c = (1LL * p.first * ((2LL * p.second % MOD - p.first + MOD) % MOD)) % MOD;
  int d = (1LL * p.first * p.first % MOD + 1LL * p.second * p.second % MOD) % MOD;
  if (n & 1) return {d, (c + d) % MOD};
  return {c, d};
}
int Mfib(int n) {
    if (n == 0) return 0;

    const int MOD = 1e9 + 7;

    struct Matrix {
        int a, b, c, d;
    };

    auto multiply = [&](Matrix x, Matrix y) {
        return Matrix{
            (x.a * y.a + x.b * y.c) % MOD,
            (x.a * y.b + x.b * y.d) % MOD,
            (x.c * y.a + x.d * y.c) % MOD,
            (x.c * y.b + x.d * y.d) % MOD
        };
    };

    auto power = [&](Matrix base, long long exp) {
        Matrix result{1, 0, 0, 1}; // identity
        while (exp > 0) {
            if (exp & 1) result = multiply(result, base);
            base = multiply(base, base);
            exp >>= 1;
        }
        return result;
    };

    Matrix M{1, 1, 1, 0};
    Matrix Mn = power(M, n);

    return Mn.b; // F_n
}
