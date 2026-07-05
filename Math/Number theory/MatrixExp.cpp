const int MOD = 1e9 + 7;

// Upgraded to support R x C dimensions
struct Matrix {
  vector<vector<int>> mat;
  int r, c;
  Matrix(int rows, int cols) : r(rows), c(cols) {
    mat.assign(r, vector<int>(c, 0));
  }
  // Identity matrix must be square (n x n)
  static Matrix identity(int n) {
    Matrix res(n, n);
    for (int i = 0; i < n; i++) {
      res.mat[i][i] = 1;
    }
    return res;
  }
};

// Extracted mul function that handles A(r x c) * B(c x k)
Matrix mul(const Matrix& a, const Matrix& b) {
  Matrix res(a.r, b.c);
  for (int i = 0; i < a.r; i++) {
    for (int k = 0; k < a.c; k++) {
      if (a.mat[i][k] == 0) continue; // Optimization
      for (int j = 0; j < b.c; j++) {
        res.mat[i][j] = (res.mat[i][j] + a.mat[i][k] * b.mat[k][j]) % MOD;
      }
    }
  }
  return res;
}

// Power function uses the new mul()
Matrix power(Matrix a, int b) {
  Matrix res = Matrix::identity(a.r);
  while (b > 0) {
    if (b & 1) res = mul(res, a);
    a = mul(a, a);
    b >>= 1;
  }
  return res;
}
