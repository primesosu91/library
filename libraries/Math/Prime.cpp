#include <bits/stdc++.h>
using namespace std;

struct Prime {
private:
    using u64 = unsigned long long;
    using u128 = __uint128_t;

    int limit;
    vector<int> spf;
    vector<int> prime_list;
    mutable u64 random_state;

    // 64-bit 整数でオーバーフローしない剰余乗算・累乗
    static u64 mul_mod(u64 a, u64 b, u64 mod) {
        return (u64)((u128)a * b % mod);
    }

    static u64 pow_mod(u64 a, u64 b, u64 mod) {
        u64 result = 1;
        while (b > 0) {
            if (b & 1) result = mul_mod(result, a, mod);
            a = mul_mod(a, a, mod);
            b >>= 1;
        }
        return result;
    }

    // 64-bit 整数に対する決定的 Miller-Rabin 判定
    static bool miller_rabin(u64 n) {
        if (n < 2) return false;
        for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
            if (n % p == 0) return n == p;
        }

        u64 d = n - 1;
        int s = __builtin_ctzll(d);
        d >>= s;
        for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
            a %= n;
            if (a == 0) continue;
            u64 x = pow_mod(a, d, n);
            if (x == 1 || x == n - 1) continue;
            bool composite = true;
            for (int i = 1; i < s; i++) {
                x = mul_mod(x, x, n);
                if (x == n - 1) {
                    composite = false;
                    break;
                }
            }
            if (composite) return false;
        }
        return true;
    }

    // Pollard's Rho で使用する擬似乱数 (SplitMix64)
    u64 next_random() const {
        u64 x = (random_state += 0x9e3779b97f4a7c15ULL);
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    // Brent 法による非自明な因数の探索 (n は合成数)
    u64 pollard_rho(u64 n) const {
        if (n % 2 == 0) return 2;
        if (n % 3 == 0) return 3;
        while (true) {
            u64 y = next_random() % (n - 1) + 1;
            u64 c = next_random() % (n - 1) + 1;
            auto f = [&](u64 x) {
                u64 z = mul_mod(x, x, n);
                return z >= n - c ? z - (n - c) : z + c;
            };

            u64 g = 1, r = 1, q = 1, x = 0, ys = 0;
            while (g == 1) {
                x = y;
                for (u64 i = 0; i < r; i++) y = f(y);
                for (u64 k = 0; k < r && g == 1;) {
                    ys = y;
                    u64 len = min(128ULL, r - k);
                    for (u64 i = 0; i < len; i++) {
                        y = f(y);
                        u64 delta = x > y ? x - y : y - x;
                        q = mul_mod(q, delta, n);
                    }
                    g = gcd(q, n);
                    k += len;
                }
                r *= 2;
            }

            if (g == n) {
                do {