// Function to calculate the total sum of set bits from 1 to n
int countSetBits(long long n) {
    if (n <= 0) {
        return 0;
    }
    int x = 0;
    while ((1LL << x) <= n) {
        x++;
    }
    x--; 
    int bits_up_to_power = x * (1LL << (x - 1));
    int msbs_from_power_to_n = n - (1LL << x) + 1;
    int rest = countSetBits(n - (1LL << x));
    return bits_up_to_power + msbs_from_power_to_n + rest;
}
int count(int n){
    int res = 0;
    for (int i = 0; i < 60; i++) {
        int bit = 1LL << i;
        int cycle = bit << 1LL;
        int full = n / cycle;
        int rem = n % cycle;
        res += full * bit;
        res += max(0LL, rem - bit + 1);
    }
    cout << res << '\n';
}
