/* 
 * Range Prime Factorization (Segmented Sieve Approach)
 * Finds the prime factorization for every number in a range [L, R].
 * Time Complexity: O((R - L) * log(log(sqrt(R))) + sqrt(R)) 
 * -> Extremely fast for large bounds (e.g., R <= 10^12) as long as the range size (R - L) is manageable (e.g., <= 10^5).
 * 
 * Core Idea: 
 * 1. Precompute prime numbers up to sqrt(R) using a standard sieve.
 * 2. Map the range [L, R] to a 0-indexed array `nums` of size (R - L + 1).
 * 3. For each prime, find its first multiple in the range using the jump formula: start_idx = (p - (L % p)) % p.
 * 4. Jump step-by-step by 'p' across the array. For each multiple, divide it out to find the prime's exponent.
 * 5. After processing all primes up to sqrt(R), any remaining value in `nums` greater than 1 is a large prime itself.
 * 
 * Returns: vector<vector<pair<long long, int>>>
 * -> Outer list represents numbers in the range (index i = L + i).
 * -> Inner list contains pairs of (prime_factor, power).
 */

// Helper function: Generates primes up to the given limit
vector<int> sieve(int limit) {
  vector<bool> is_prime(limit + 1, true);
  vector<int> primes;
  for (int p = 2; p * p <= limit; p++) {
    if (is_prime[p]) {
      for (int i = p * p; i <= limit; i += p)
        is_prime[i] = false;
    }
  }
  for (int p = 2; p <= limit; p++) {
    if (is_prime[p] && p > 1) primes.push_back(p);
  }
  return primes;
}

vector<vector<pair<int, int>>> range_prime_factorization(int L, int R) {
  int limit = sqrt(R) + 1;
  vector<int> primes = sieve(limit);
  int size = R - L + 1;
  vector<int> nums(size);
  for (int i = 0; i < size; i++) {
    nums[i] = L + i;
  }
  vector<vector<pair<int, int>>> factors(size);
  for (int p : primes) {
    int st = (p - (L % p)) % p;
    for (int i = st; i < size; i += p) {
      int power = 0;
      while (nums[i] % p == 0) {
        power++;
        nums[i] /= p;
      }
      factors[i].push_back({p, power});
    }
  }
  for (int i = 0; i < size; i++) {
    if (nums[i] > 1) {
      factors[i].push_back({nums[i], 1});
    }
  }
  return factors;
}

vector<int> PrimeFactors(int n) {
  vector<int> ret;
  for (int i = 2; i * i <= n; i++) { 
    while (n % i == 0) {
      ret.push_back(i);
      n = n / i;
    }
  }
  if (n > 1) { 
    ret.push_back(n);
  }
  return ret;
}
