// Generate all primes up to limit using sieve of eratosthenes
vector<int> sieve(int limit) {
  vector<bool> is_prime(limit + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int p = 2; p * p <= limit; ++p) {
    if (is_prime[p]) {
      for (int i = p * p; i <= limit; i += p) {
        is_prime[i] = false;
      }
    }
  }
  vector<int> primes;
  for (int p = 2; p <= limit; ++p) {
    if (is_prime[p]) {
      primes.push_back(p);
    }
  }
  return primes;
}

// Generate all primes from l to r using segmented sieve in O((r - l) log (r) + sqrt(r))
vector<int> segmented_sieve(int l, int r) {
  if (l == 1) {
    l++;
  }
  int limit = sqrtl(r);
  while ((int) limit * limit <= r) limit++;
  while ((int) limit * limit > r) limit--;
  auto primes = sieve(limit);
  vector<bool> is_prime(r - l + 1, true);
  for (int p : primes) {
    int start = max((int)p * p, (int)(l + p - 1) / p * p);
    for (int j = start; j <= r; j += p) {
      is_prime[j - l] = false;
    }
  }
  vector<int> vec;
  for (int i = l; i <= r; ++i) {
    if (is_prime[i - l]) {
      vec.push_back(i);
    }
  }
  return vec;
}
