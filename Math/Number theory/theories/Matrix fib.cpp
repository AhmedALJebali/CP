int fib(int n) {
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
