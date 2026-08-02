// returns the count of integers coprime to a single value n
int phi(int n) {
  int result = n;
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      while (n % i == 0)
        n /= i;
      result -= result / i;
    }
  }
  if (n > 1)
    result -= result / n;
  return result;
}
// precomputes an array phi containing these counts for all numbers up to MAXN.
const int MAXN = 1e6+5;
int phi[MAXN];
void totient_sieve() {
    for (int i = 0; i < MAXN; i++) phi[i] = i;
    for (int i = 2; i < MAXN; i++) {
        if (phi[i] == i) {
            for (int j = i; j < MAXN; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }
}
