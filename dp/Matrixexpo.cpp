const int MOD=1e9+7;
const int INF=2e18;
struct Matrix {
  vector<vector<int>> mat;
  int r, c;
  Matrix(int rows, int cols) : r(rows), c(cols) {
    mat.assign(r, vector<int>(c, 0));
  }
 
  static Matrix identity(int n) {
    Matrix res(n, n);
    for (int i = 0; i < n; i++) {
      res.mat[i][i] = 1;
    }
    return res;
  }
 
  void set(int i, int j, int x) {
    mat[i][j] += (x % MOD + MOD) % MOD;
  }
 
  int get(int i, int j) const {
    return mat[i][j];
  }
};
Matrix mul(const Matrix &a, const Matrix &b) {
  Matrix res(a.r, b.c);
  for (int i = 0; i < a.r; i++) {
    for (int j = 0; j < b.c; j++) {
      int sum = 0;
      for (int k = 0; k < a.c; k++) {
        sum = (sum + a.get(i, k) * b.get(k, j) % MOD) % MOD;
      }
      res.set(i, j, sum);
    }
  }
  return res;
}
Matrix Fast(Matrix a, int base) {
  Matrix res = Matrix::identity(a.r);
  while (base) {
    if (base & 1) res = mul(res, a);
    a = mul(a, a);
    base >>= 1;
  }
  return res;
}
 
