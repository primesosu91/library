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
                    ys = f(ys);
                    u64 delta = x > ys ? x - ys : ys - x;
                    g = gcd(delta, n);
                } while (g == 1);
            }
            if (g != n) return g;
        }
    }

    // 素因数を重複込みで求める
    void factor_rec(u64 n, vector<u64>& factors) const {
        if (n == 1) return;
        if (n <= (u64)limit) {
            while (n > 1) {
                int p = spf[(int)n];
                factors.push_back(p);
                n /= p;
            }
            return;
        }
        if (miller_rabin(n)) {
            factors.push_back(n);
            return;
        }
        u64 d = pollard_rho(n);
        factor_rec(d, factors);
        factor_rec(n / d, factors);
    }

public:
    // 初期化: Prime(n) -> [0, n] の最小素因数と素数一覧を構築 (前提: n >= 0)
    Prime(int n = 0) : limit(0), random_state((u64)chrono::steady_clock::now().time_since_epoch().count()) {
        build(n);
    }

    // 再構築: build(n) -> [0, n] を前計算し直す (前提: n >= 0)
    void build(int n) {
        assert(n >= 0);
        limit = n;
        spf.assign((size_t)n + 1, 0);
        prime_list.clear();
        for (int i = 2; i <= n; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                prime_list.push_back(i);
            }
            for (int p : prime_list) {
                if (p > spf[i] || (long long)p * i > n) break;
                spf[p * i] = p;
            }
        }
    }

    // 素数判定: is_prime(n) -> n が素数なら true (n <= 1 は false)
    bool is_prime(long long n) const {
        if (n < 2) return false;
        if (n <= limit) return spf[(int)n] == n;
        return miller_rabin((u64)n);
    }

    // 素数一覧: primes() -> 前計算範囲 [2, N] の素数への参照
    const vector<int>& primes() const {
        return prime_list;
    }

    // 素数列挙: enumerate(n) -> n 以下の全素数 (前提: 0 <= n <= N)
    vector<int> enumerate(int n) const {
        assert(0 <= n && n <= limit);
        auto it = upper_bound(prime_list.begin(), prime_list.end(), n);
        return vector<int>(prime_list.begin(), it);
    }

    // 区間素数列挙: enumerate(l, r) -> [l, r) の素数 (前提: 0 <= l <= r)
    vector<long long> enumerate(long long l, long long r) const {
        assert(0 <= l && l <= r);
        vector<long long> result;
        l = max(l, 2LL);
        if (l >= r) return result;

        if (r - 1 <= limit) {
            auto first = lower_bound(prime_list.begin(), prime_list.end(), l);
            auto last = lower_bound(first, prime_list.end(), r);
            result.insert(result.end(), first, last);
            return result;
        }

        // √(r-1) 以下の素数が前計算済みなら、分割区間篩を使う
        if (limit >= 2 && (long long)limit * limit >= r - 1) {
            constexpr long long block_size = 1 << 20;
            while (l < r) {
                long long high = r - l > block_size ? l + block_size : r;
                vector<char> is_prime_block((size_t)(high - l), true);
                for (int p : prime_list) {
                    if ((long long)p * p >= high) break;
                    u128 first = max((u128)p * p, (((u128)l + p - 1) / p) * p);
                    if (first >= (u128)high) continue;
                    for (long long x = (long long)first; x < high;) {
                        is_prime_block[(size_t)(x - l)] = false;
                        if (high - x <= p) break;
                        x += p;
                    }
                }
                for (long long i = 0; i < high - l; i++) {
                    if (is_prime_block[(size_t)i]) result.push_back(l + i);
                }
                l = high;
            }
            return result;
        }

        // 範囲外では決定的 Miller-Rabin 法を用いる
        if (l <= 2) {
            result.push_back(2);
            l = 3;
        }
        if (l % 2 == 0) l++;
        while (l < r) {
            if (is_prime(l)) result.push_back(l);
            if (r - l <= 2) break;
            l += 2;
        }
        return result;
    }

    // 素数個数: count(n) -> n 以下の素数の個数 π(n) (前提: 0 <= n <= N)
    int count(int n) const {
        assert(0 <= n && n <= limit);
        return (int)(upper_bound(prime_list.begin(), prime_list.end(), n) - prime_list.begin());
    }

    // 素数総数: count() -> N 以下の素数の総数
    int count() const {
        return (int)prime_list.size();
    }

    // k 番目の素数: kth_prime(k) -> 0-index の素数 (前提: 0 <= k < count())
    int kth_prime(int k) const {
        assert(0 <= k && k < count());
        return prime_list[k];
    }

    // 最小素因数: smallest_prime_factor(n) -> n の最小素因数 (前提: 2 <= n <= N)
    int smallest_prime_factor(int n) const {
        assert(2 <= n && n <= limit);
        return spf[n];
    }

    // 素因数分解: factorize(n) -> {素因数, 指数} を昇順で返す (前提: n >= 1、n=1 では空)
    vector<pair<long long, int>> factorize(long long n) const {
        assert(n >= 1);
        vector<u64> factors;
        factor_rec((u64)n, factors);
        sort(factors.begin(), factors.end());
        vector<pair<long long, int>> result;
        for (u64 p : factors) {
            if (result.empty() || result.back().first != (long long)p) result.emplace_back((long long)p, 1);
            else result.back().second++;
        }
        return result;
    }

    // 約数列挙: divisors(n) -> 正の約数を昇順に返す (前提: n >= 1)
    vector<long long> divisors(long long n) const {
        assert(n >= 1);
        auto factors = factorize(n);
        vector<long long> result = {1};
        for (auto [p, e] : factors) {
            size_t size = result.size();
            long long power = 1;
            for (int i = 0; i < e; i++) {
                power *= p;
                for (size_t j = 0; j < size; j++) result.push_back(result[j] * power);
            }
        }
        sort(result.begin(), result.end());
        return result;
    }

    // 約数個数: divisor_count(n) -> 正の約数の個数 (前提: n >= 1)
    long long divisor_count(long long n) const {
        assert(n >= 1);
        long long result = 1;
        for (auto [p, e] : factorize(n)) result *= e + 1;
        return result;
    }

    // 約数総和: divisor_sum(n) -> 正の約数の総和 (前提: n >= 1、総和が long long に収まる)
    long long divisor_sum(long long n) const {
        assert(n >= 1);
        u128 result = 1;
        for (auto [p, e] : factorize(n)) {
            u128 term = 1, power = 1;
            for (int i = 0; i < e; i++) {
                power *= (u64)p;
                term += power;
            }
            result *= term;
        }
        assert(result <= (u128)LLONG_MAX);
        return (long long)result;
    }

    // Euler の φ 関数: euler_phi(n) -> 1 <= k <= n かつ gcd(k,n)=1 の整数 k の個数 (前提: n >= 1)
    long long euler_phi(long long n) const {
        assert(n >= 1);
        long long result = n;
        for (auto [p, e] : factorize(n)) result = result / p * (p - 1);
        return result;
    }

    // Möbius 関数: mobius(n) -> μ(n) ∈ {-1,0,1} (前提: n >= 1)
    int mobius(long long n) const {
        assert(n >= 1);
        int result = 1;
        for (auto [p, e] : factorize(n)) {
            if (e >= 2) return 0;
            result = -result;
        }
        return result;
    }
};
