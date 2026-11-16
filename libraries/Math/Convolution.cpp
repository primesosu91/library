#include <bits/stdc++.h>
using namespace std;

namespace convolution_internal {
    // 内部用NTT。aの長さは2冪かつ2^23以下。inverseなら逆変換する。
    template<class Mint>
    void ntt(vector<Mint>& a, bool inverse) {
        int n = (int)a.size();

        // 添字をビット反転順に並べる。
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            while (j & bit) {
                j ^= bit;
                bit >>= 1;
            }
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }

        // 原始根3から各段の1の冪根を求め、バタフライ演算を行う。
        Mint primitive_root = Mint(3);
        if (inverse) primitive_root = primitive_root.inv();
        for (int length = 2; length <= n; length <<= 1) {
            Mint root = primitive_root.pow((Mint::mod() - 1) / length);
            int half = length / 2;
            for (int start = 0; start < n; start += length) {
                Mint factor = 1;
                for (int j = 0; j < half; j++) {
                    Mint left = a[start + j];
                    Mint right = a[start + j + half] * factor;
                    a[start + j] = left + right;
                    a[start + j + half] = left - right;
                    factor *= root;
                }
            }
        }

        // 逆変換を正規化する。
        if (inverse) {
            Mint inverse_n = Mint(n).inv();
            for (Mint& value : a) value *= inverse_n;
        }
    }
}

// 畳み込み: convolution(a,b) -> c[k] = sum_{i+j=k} a[i]*b[j]。
// 係数は次数の昇順。入力は変更せず、末尾0も保持。片方が空なら空を返す。
// 前提: Mint::mod() == 998244353、両方非空なら a.size()+b.size()-1 <= 2^23。
// Mintは整数からの構築、加減乗算、pow、inv、constexprなmodに対応する型。
// 短い方が60以下なら時間O(n*m)、それ以外はO(L log L)。追加領域O(n+m)。
// Lはn+m-1以上の最小の2冪。使用例: auto c = convolution(a, b);
template<class Mint>
vector<Mint> convolution(const vector<Mint>& a, const vector<Mint>& b) {
    static_assert(Mint::mod() == 998244353, "convolution requires modulus 998244353");
    if (a.empty() || b.empty()) return {};

    // サイズの加算・int変換より先に上限を確認する。
    constexpr size_t max_size = size_t(1) << 23;
    assert(a.size() <= max_size && b.size() <= max_size);
    assert(a.size() - 1 <= max_size - b.size());
    int n = (int)a.size(), m = (int)b.size();
    int result_size = n + m - 1;

    // 小さい入力では変換の準備を省き、直接計算する。
    if (min(n, m) <= 60) {
        vector<Mint> result(result_size, Mint(0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) result[i + j] += a[i] * b[j];
        }
        return result;
    }

    int size = 1;
    while (size < result_size) size <<= 1;
    vector<Mint> left(a), right(b);
    left.resize(size, Mint(0));
    right.resize(size, Mint(0));
    convolution_internal::ntt(left, false);
    convolution_internal::ntt(right, false);
    for (int i = 0; i < size; i++) left[i] *= right[i];
    convolution_internal::ntt(left, true);
    left.resize(result_size);
    return left;
}
