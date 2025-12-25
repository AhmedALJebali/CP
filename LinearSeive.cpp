const int N = 1e7;

int lp[N + 1];
bool isPrime[N + 1];
vector<int> primes;

void linear_sieve(int n) {
    for (int i = 2; i <= n; i++) {
        if (lp[i] == 0) {
            lp[i] = i;
            isPrime[i] = true;
            primes.push_back(i);
        }
        for (int p: primes) {
            if (p > lp[i] || i * p > n) break;
            lp[i * p] = p;
            isPrime[i * p] = false;
        }
    }
}
