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
vector<int> totient_sieve(int n) {
  vector<int> phi(n);
  for (int i = 0; i < n; i++) phi[i] = i;
  for (int i = 2; i < n; i++) {
    if (phi[i] == i) {
      for (int j = i; j < n; j += i) {
        phi[j] -= phi[j] / i;
      }
    }
  }
  return phi;
}

vector<int> phi_1_to_n(int n) {
  vector<int> phi(n + 1);
  phi[0] = 0;
  phi[1] = 1;
  for (int i = 2; i <= n; i++)
    phi[i] = i - 1;
  for (int i = 2; i <= n; i++)
    for (int j = 2 * i; j <= n; j += i)
      phi[j] -= phi[i];

  return phi;
}


// To get the value of phi(x) for L <= x <= R, use phi[x - L].
const int MAX_RANGE = 1e6 + 6;
vector<int> primes;
int phi[MAX_RANGE], rem[MAX_RANGE];

vector<int> linear_sieve(int n) { 
  vector<bool> composite(n + 1, 0);
  vector<int> prime;
  // 0 and 1 are not composite (nor prime)
  composite[0] = composite[1] = 1;
  for(int i = 2; i <= n; i++) {
    if(!composite[i]) prime.push_back(i);
    for(int j = 0; j < prime.size() && i * prime[j] <= n; j++) {
      composite[i * prime[j]] = true;
      if(i % prime[j] == 0) break;
    }
  }
  return prime;
}
void segmented_phi(int L, int R) { 
  for(int i = L; i <= R; i++) {
    rem[i - L] = i;
    phi[i - L] = i;
  }
  for(int i : primes) {
    for(int j = max(i * i, (L + i - 1) / i * i); j <= R; j += i) {
      phi[j - L] -= phi[j - L] / i;
      while(rem[j - L] % i == 0) rem[j - L] /= i;
    }
  }
  for(int i = 0; i < R - L + 1; i++) {
    if(rem[i] > 1) phi[i] -= phi[i] / rem[i];
  }
}
