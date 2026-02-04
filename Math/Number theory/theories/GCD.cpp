int gcd(int a, int b) {
  while (b) {
    a %= b;
    swap(a, b);
  }
  return a;
}

int gcd_rec(int a, int b) {
  return b ? gcd(b, a % b) : a;
}
