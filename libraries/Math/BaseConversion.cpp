#include <bits/stdc++.h>
using namespace std;

// 非負整数を base 進法の上位桁からの桁列へ変換。0 は {0} を返す
// 引数: n は変換する整数、base は基数 (前提: n >= 0、2 <= base <= INT_MAX)
// 桁数 k に対して時間 O(k)、返り値を含む追加領域 O(k)
vector<int> to_base(long long n, int base) {
    assert(n >= 0 && base >= 2);

    // 下位桁から取り出し、上位桁からの順序に直す
    vector<int> result;
    do {
        result.push_back((int)(n % base));
        n /= base;
    } while (n > 0);
    reverse(result.begin(), result.end());
    return result;
}

// base 進法の上位桁からの桁列を非負整数へ復元。先頭の 0 を許容
// 引数: digits は空でない桁列、base は基数 (前提: base >= 2、各桁は [0, base))
// 戻り値: long long の範囲内の値 (前提)。時間 O(桁数)、追加領域 O(1)
long long from_base(const vector<int>& digits, int base) {
    assert(base >= 2 && !digits.empty());

    // Horner法で復元し、乗算・加算の前に範囲を検査する
    long long result = 0;
    for (int digit : digits) {
        assert(0 <= digit && digit < base);
        assert(result <= (LLONG_MAX - digit) / base);
        result = result * base + digit;
    }
    return result;
}

// 非負整数を base 進法の文字列へ変換。上位桁から 0-9・A-Z を使い、0 は "0" を返す
// 引数: n は変換する整数、base は基数 (前提: n >= 0、2 <= base <= 36)
// 桁数 k に対して時間 O(k)、返り値を含む追加領域 O(k)
string to_base_string(long long n, int base) {
    assert(n >= 0 && 2 <= base && base <= 36);

    const string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result;
    do {
        result.push_back(alphabet[n % base]);
        n /= base;
    } while (n > 0);
    reverse(result.begin(), result.end());
    return result;
}

// base 進法の文字列を非負整数へ復元。英字は大小両方、先頭の 0 を許容
// 引数: s は上位桁からの空でない文字列、base は基数 (前提: 2 <= base <= 36、各桁が有効)
// 戻り値: long long の範囲内の値 (前提)。符号・空白は不可。時間 O(桁数)、追加領域 O(1)
long long from_base_string(const string& s, int base) {
    assert(2 <= base && base <= 36 && !s.empty());

    long long result = 0;
    for (char c : s) {
        // 文字を桁の値に変換し、乗算・加算の前に範囲を検査する
        int digit = -1;
        if ('0' <= c && c <= '9') digit = c - '0';
        else if ('A' <= c && c <= 'Z') digit = c - 'A' + 10;
        else if ('a' <= c && c <= 'z') digit = c - 'a' + 10;
        assert(0 <= digit && digit < base);
        assert(result <= (LLONG_MAX - digit) / base);
        result = result * base + digit;
    }
    return result;
}
