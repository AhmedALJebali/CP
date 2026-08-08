// Calculates ways to choose x places in a row with at least 'g' spaces between them
int ways_in_row(int n, int x, int g) {
  if (x < 0) return 0;
  if (x == 0) return 1;
  int space_needed = x + (x - 1) * g;
  if (space_needed > n) {
    return 0;
  }
  return nCr(n - (x - 1) * g, x);
}
// Calculates ways to choose x places in a circle with at least 'g' spaces between them
int ways_in_circle(int n, int x, int g) {
  if (x < 0) return 0;
  if (x == 0) return 1;
  int space_needed = x + (x * g);
  if (space_needed > n) {
    return 0;
  }
  if (x == 1) return n;
  return nCr(n - x * g, x) + g * nCr(n - x * g - 1, x - 1);
}
