// Returns the exact total number of prime numbers less than or equal to 'n' (the prime-counting function pi(n)).
// Time Complexity: O(n^(3/4)) | Space Complexity: O(sqrt(n)) | Handles n up to 10^11.
int count_primes(int n) {
  if (n <= 1) return 0;
  int v = sqrt(n);
  vector<int> S(v + 1);
  vector<int> L(v + 1);
  for (int i = 1; i <= v; i++) {
    S[i] = i - 1;
    L[i] = n / i - 1;
  }
  for (int p = 2; p <= v; p++) {
    if (S[p] > S[p - 1]) { // p is a prime
      int sp = S[p - 1];
      int p2 = p * p;
      int end = min(v, n / p2);
      for (int i = 1; i <= end; i++) {
        int d = i * p;
        if (d <= v) {
          L[i] -= (L[d] - sp);
        } else {
          L[i] -= (S[n / d] - sp);
        }
      }
      for (int i = v; i >= p2; i--) {
        S[i] -= (S[i / p] - sp);
      }
    }
  }
  return L[1];
}
