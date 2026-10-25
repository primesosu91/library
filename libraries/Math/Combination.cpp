#include <bits/stdc++.h>
using namespace std;

template<class Mint>
struct Combination {
private:
    int limit;
    vector<Mint> factorial, inverse_factorial;

    // 素数MODであることを初期化時に一度だけ確認する
    static bool is_prime_mod() {
        int p = Mint::mod();
        if (p < 2) return false;
        for (int d = 2; (long long)d * d <= p; d++) {
            if (p % d == 0) return false;
        }
        return true;
    }

    // 前計算範囲外も積で求める階乗 (前提: 0 <= n < MOD)
    Mint factorial_value(long long n) const {
        if (n <= limit) return factorial[(int)n];
        Mint result = factorial[limit];
        for (long long i = (long long)limit + 1; i <= n; i++) result *= Mint(i);
        return result;
    }

    // 1桁の組合せ数。前計算・乗算・MOD直前の恒等式を使い分ける
    Mint comb_digit(long long n, long long r) const {
        if (r > n) return Mint(0);
        if (n <= limit) {
            return factorial[(int)n] * inverse_factorial[(int)r]
                   * inverse_factorial[(int)(n - r)];
        }
        long long k = min(r, n - r);
        long long a = (long long)Mint::mod() - 1 - n;
        if (a < k) {
            // C(p-1-a,k) = (-1)^k C(k+a,a) (mod p)
            Mint result = 1;
            for (long long i = 1; i <= a; i++) result *= Mint(k + i);
            if (a <= limit) result *= inverse_factorial[(int)a];
            else result /= factorial_value(a);
            if (k & 1) result = -result;
            return result;
        }
        Mint result = 1;
        for (long long i = 1; i <= k; i++) result *= Mint(n - k + i);
        if (k <= limit) result *= inverse_factorial[(int)k];
        else result /= factorial_value(k);
        return result;
    }

    // Lucasの定理。重複組合せの中間値がlong longを超える場合にも対応
    Mint comb_large(unsigned __int128 n, unsigned __int128 r) const {
        const unsigned __int128 p = (unsigned int)Mint::mod();
        Mint result = 1;
        while (n > 0 || r > 0) {
            long long ni = (long long)(n % p);
            long long ri = (long long)(r % p);
            if (ri > ni) return Mint(0);
            result *= comb_digit(ni, ri);
            n /= p;
            r /= p;
        }
        return result;
    }

public:
    // 初期化: Combination(n) -> 0..n の階乗・逆階乗を前計算 (前提: MODは素数、0 <= n < MOD)
    Combination(int n = 0) {
        static const bool prime = is_prime_mod();
        assert(prime);
        build(n);
    }

    // 再構築: build(n) -> 前計算範囲を0..nに変更 (前提: 0 <= n < MOD)
    void build(int n) {
        assert(0 <= n && n < Mint::mod());
        limit = n;
        factorial.assign(n + 1, Mint(1));
        inverse_factorial.assign(n + 1, Mint(1));
        for (int i = 1; i <= n; i++) factorial[i] = factorial[i - 1] * Mint(i);
        inverse_factorial[n] = factorial[n].inv();
        for (int i = n; i >= 1; i--) inverse_factorial[i - 1] = inverse_factorial[i] * Mint(i);
    }

    // 組合せ数: comb(n,r) -> C(n,r) mod MOD (前提: n >= 0、MODは素数; r<0 または r>n は0)
    Mint comb(long long n, long long r) const {
        assert(n >= 0);
        if (r < 0 || r > n) return Mint(0);
        return comb_large((unsigned long long)n, (unsigned long long)r);
    }

    // 順列数: perm(n,r) -> nPr mod MOD (前提: n >= 0、MODは素数; r<0 または r>n は0)
    Mint perm(long long n, long long r) const {
        assert(n >= 0);
        if (r < 0 || r > n) return Mint(0);
        if (r == 0) return Mint(1);
        long long x = n % Mint::mod();
        if (r > x) return Mint(0); // 連続するr項の中にMODの倍数がある
        if (x <= limit) return factorial[(int)x] * inverse_factorial[(int)(x - r)];
        long long a = (long long)Mint::mod() - 1 - x;
        long long b = x - r;
        if (r <= a + b) {
            Mint result = 1;
            for (long long i = 0; i < r; i++) result *= Mint(x - i);
            return result;
        }
        // Wilsonの定理: x!/(x-r)! = (-1)^(a+1)/(a! b!) (mod p)
        Mint result = (factorial_value(a) * factorial_value(b)).inv();
        if ((a + 1) & 1) result = -result;
        return result;
    }

    // 重複組合せ数: hcomb(n,r) -> H(n,r) mod MOD (前提: n >= 0; r<0 は0、H(0,0)=1)
    Mint hcomb(long long n, long long r) const {
        assert(n >= 0);
        if (r < 0) return Mint(0);
        if (n == 0) return Mint(r == 0 ? 1 : 0);
        return comb_large((unsigned __int128)(unsigned long long)n
                          + (unsigned long long)r - 1, (unsigned long long)r);
    }

    // 階乗: fact(n) -> n! mod MOD (前提: 0 <= n <= 前計算上限)
    Mint fact(int n) const {
        assert(0 <= n && n <= limit);
        return factorial[n];
    }

    // 逆階乗: inv_fact(n) -> (n!)^(-1) mod MOD (前提: 0 <= n <= 前計算上限)
    Mint inv_fact(int n) const {
        assert(0 <= n && n <= limit);
        return inverse_factorial[n];
    }

    // 逆元: inv(n) -> n^(-1) mod MOD (前提: n >= 1、n % MOD != 0)
    Mint inv(long long n) const {
        assert(n >= 1 && n % Mint::mod() != 0);
        int x = (int)(n % Mint::mod());
        if (x <= limit) return factorial[x - 1] * inverse_factorial[x];
        return Mint(x).inv();
    }
};
