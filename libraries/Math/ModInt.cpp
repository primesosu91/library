#include <bits/stdc++.h>
using namespace std;

template<int MOD>
struct ModInt {
    static_assert(MOD >= 2, "MOD must be at least 2");

private:
    int value;

public:
    // 初期化: ModInt() -> 0
    ModInt() : value(0) {}

    // 初期化: ModInt(x) -> 整数 x を [0, MOD) に正規化
    template<class T, enable_if_t<is_integral_v<T>, int> = 0>
    ModInt(T x) {
        if constexpr (is_signed_v<T>) {
            long long y = (long long)x % MOD;
            if (y < 0) y += MOD;
            value = (int)y;
        } else {
            value = (int)((unsigned long long)x % (unsigned int)MOD);
        }
    }

    // 値取得: val() -> [0, MOD) の整数
    int val() const {
        return value;
    }

    // 法の取得: mod() -> MOD
    static constexpr int mod() {
        return MOD;
    }

    // 正規化済みの値から生成: raw(x) -> x (前提: 0 <= x < MOD)
    static ModInt raw(int x) {
        assert(0 <= x && x < MOD);
        ModInt result;
        result.value = x;
        return result;
    }

    // 加算代入: a += b -> (a + b) mod MOD
    ModInt& operator+=(const ModInt& other) {
        unsigned int x = (unsigned int)value + (unsigned int)other.value;
        if (x >= (unsigned int)MOD) x -= MOD;
        value = (int)x;
        return *this;
    }

    // 減算代入: a -= b -> (a - b) mod MOD
    ModInt& operator-=(const ModInt& other) {
        value -= other.value;
        if (value < 0) value += MOD;
        return *this;
    }

    // 乗算代入: a *= b -> (a * b) mod MOD
    ModInt& operator*=(const ModInt& other) {
        value = (int)((long long)value * other.value % MOD);
        return *this;
    }

    // 除算代入: a /= b -> a * b.inv() (前提: gcd(b, MOD) == 1)
    ModInt& operator/=(const ModInt& other) {
        return *this *= other.inv();
    }

    // 累乗: pow(n) -> a^n mod MOD (前提: n >= 0)
    ModInt pow(long long n) const {
        assert(n >= 0);
        ModInt result = 1;
        ModInt base = *this;
        while (n > 0) {
            if (n & 1) result *= base;
            base *= base;
            n >>= 1;
        }
        return result;
    }

    // 逆元: inv() -> a^(-1) mod MOD (前提: gcd(a, MOD) == 1)
    ModInt inv() const {
        long long a = value;
        long long b = MOD;
        long long x = 1;
        long long y = 0;

        // 拡張Euclid互除法で a * x + MOD * y = gcd(a, MOD) を求める
        while (b != 0) {
            long long q = a / b;
            a -= q * b;
            swap(a, b);
            x -= q * y;
            swap(x, y);
        }
        assert(a == 1);
        return ModInt(x);
    }

    // 単項演算: +a, -a -> 正符号、加法逆元
    ModInt operator+() const { return *this; }
    ModInt operator-() const { return value == 0 ? *this : raw(MOD - value); }

    // 前置・後置インクリメント: ++a, a++ -> 1 加算
    ModInt& operator++() {
        value++;
        if (value == MOD) value = 0;
        return *this;
    }
    ModInt operator++(int) {
        ModInt result = *this;
        ++*this;
        return result;
    }

    // 前置・後置デクリメント: --a, a-- -> 1 減算
    ModInt& operator--() {
        if (value == 0) value = MOD;
        value--;
        return *this;
    }
    ModInt operator--(int) {
        ModInt result = *this;
        --*this;
        return result;
    }

    // 四則演算: a + b, a - b, a * b, a / b -> 演算結果
    friend ModInt operator+(ModInt left, const ModInt& right) { return left += right; }
    friend ModInt operator-(ModInt left, const ModInt& right) { return left -= right; }
    friend ModInt operator*(ModInt left, const ModInt& right) { return left *= right; }
    friend ModInt operator/(ModInt left, const ModInt& right) { return left /= right; }

    // 比較: a == b, a != b -> 保持値の等値判定
    friend bool operator==(const ModInt& left, const ModInt& right) { return left.value == right.value; }
    friend bool operator!=(const ModInt& left, const ModInt& right) { return left.value != right.value; }

    // 出力: os << a -> 保持値を整数で出力
    friend ostream& operator<<(ostream& os, const ModInt& a) {
        return os << a.value;
    }

    // 入力: is >> a -> long long の整数を読み込み正規化
    friend istream& operator>>(istream& is, ModInt& a) {
        long long x;
        if (is >> x) a = ModInt(x);
        return is;
    }
};
