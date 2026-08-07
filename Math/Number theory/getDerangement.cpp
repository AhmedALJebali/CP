// Returns the number of derangements of 'n' items (permutations where no element remains in its original position).
vector<long long> getDerangements(int n) {
    // If n is 0, return just the base case for 0
    if (n <= 0) return {1};
    // Create a vector of size n + 1 to store answers for 0 through n
    vector<long long> D(n + 1);
    // Base cases
    D[0] = 1;
    D[1] = 0;
    // Compute all derangements from 2 up to n
    for (int i = 2; i <= n; i++) {
        D[i] = (i - 1) * (D[i - 1] + D[i - 2]);
    }
    return D;
}
