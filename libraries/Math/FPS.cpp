#include <bits/stdc++.h>
using namespace std;

// 法998244353の形式的冪級数。係数は次数の昇順、末尾0は自動削除しない。
// 依存: convolution<Mint>(vector<Mint>, vector<Mint>)を先に定義する。
// MintはModInt互換の構築・四則演算・比較・pow・inv・constexprなmodを備える。
// 保持長は2^23以下。inv/log/exp/powの指定項数は0..2^22。
template<class Mint>
struct FPS {
    static_assert(Mint::mod() == 998244353, "FPS requires modulus 998244353");

private:
    vector<Mint> data;
    static constexpr int max_size = 1 << 23;
    static constexpr int max_precision = 1 << 22;

    // 指定項数は内部の畳み込み長が上限を超えない範囲とする。
    static void check_precision(int n) {
        assert(0 <= n && n <= max_precision);
    }

public:
    // 初期化: FPS() -> 保持長0の零級数。時間O(1)。
    FPS() = default;

    // 初期化: FPS(n) -> n項すべて0 (前提: 0 <= n <= 2^23)。時間O(n)。
    explicit FPS(int n) {
        resize(n);
    }

    // 初期化: FPS(a) -> 係数列aを保持 (前提: a.size() <= 2^23)。
    // lvalueからはコピーO(n)、rvalueからは移動O(1)。
    explicit FPS(vector<Mint> a) : data(move(a)) {
        assert(data.size() <= (size_t)max_size);
    }

    // 初期化: FPS{a0,a1,...} -> 定数項から順に保持。時間O(n)。
    FPS(initializer_list<Mint> a) : FPS(vector<Mint>(a)) {}

    // 保持長: size() -> 保持している係数の個数。時間O(1)。
    int size() const {
        return (int)data.size();
    }

    // 空判定: empty() -> 保持長が0か。零級数かどうかの判定ではない。時間O(1)。
    bool empty() const {
        return data.empty();
    }

    // 係数参照: f[i] -> x^iの係数 (前提: 0 <= i < size())。時間O(1)。
    Mint& operator[](int i) {
        assert(0 <= i && i < size());
        return data[i];
    }

    // 読み取り専用の係数参照 (前提: 0 <= i < size())。時間O(1)。
    const Mint& operator[](int i) const {
        assert(0 <= i && i < size());
        return data[i];
    }

    // 係数列取得: coefficients() -> 読み取り専用参照。時間O(1)。
    const vector<Mint>& coefficients() const {
        return data;
    }

    // 長さ変更: resize(n) -> 余分な項を削除し、不足する項は0で補う。
    // 前提: 0 <= n <= 2^23。時間は最悪O(旧サイズ+n)。
    void resize(int n) {
        assert(0 <= n && n <= max_size);
        data.resize(n, Mint(0));
    }

    // 先頭項取得: pre(n) -> 正確にn項の新しい級数。不足分は0。
    // 前提: 0 <= n <= 2^23。時間・追加領域O(n)。
    FPS pre(int n) const {
        FPS result(n);
        copy_n(data.begin(), min(size(), n), result.data.begin());
        return result;
    }

    // 単項マイナス: -f -> 全係数の符号を反転。時間・追加領域O(size())。
    FPS operator-() const {
        FPS result(*this);
        for (Mint& value : result.data) value = -value;
        return result;
    }

    // 加算代入: f += g -> 長さmax(f.size(),g.size())の和。自分への参照を返す。
    // 時間O(n+m)、追加領域は最悪O(n+m)。
    FPS& operator+=(const FPS& g) {
        if (size() < g.size()) resize(g.size());
        for (int i = 0; i < g.size(); i++) data[i] += g.data[i];
        return *this;
    }

    // 減算代入: f -= g -> 長さmax(f.size(),g.size())の差。自分への参照を返す。
    // 時間O(n+m)、追加領域は最悪O(n+m)。
    FPS& operator-=(const FPS& g) {
        if (size() < g.size()) resize(g.size());
        for (int i = 0; i < g.size(); i++) data[i] -= g.data[i];
        return *this;
    }

    // 乗算代入: f *= g -> 畳み込みによる積。自分への参照を返す。
    // 片方が空なら空。両方非空ならn+m-1 <= 2^23。時間O(L log L)、追加領域O(L)。
    // Lはn+m-1以上の最小の2冪。短い入力ではConvolutionの直接計算を使用。
    FPS& operator*=(const FPS& g) {
        data = convolution(data, g.data);
        return *this;
    }

    // 定数倍代入: f *= c -> 全係数をc倍。自分への参照を返す。時間O(n)、追加領域O(1)。
    FPS& operator*=(Mint c) {
        for (Mint& value : data) value *= c;
        return *this;
    }

    // 加算: f + g -> 新しい和。時間・追加領域O(n+m)。
    FPS operator+(const FPS& g) const {
        FPS result(*this);
        result += g;
        return result;
    }

    // 減算: f - g -> 新しい差。時間・追加領域O(n+m)。
    FPS operator-(const FPS& g) const {
        FPS result(*this);
        result -= g;
        return result;
    }

    // 乗算: f * g -> 新しい積。前提・計算量は乗算代入と同じ。
    FPS operator*(const FPS& g) const {
        return FPS(convolution(data, g.data));
    }

    // 定数倍: f * c -> 全係数をc倍した新しい級数。時間・追加領域O(n)。
    FPS operator*(Mint c) const {
        FPS result(*this);
        result *= c;
        return result;
    }

    // 微分: diff() -> f'。結果長max(size()-1,0)。時間・追加領域O(size())。
    FPS diff() const {
        FPS result(max(size() - 1, 0));
        for (int i = 1; i < size(); i++) result.data[i - 1] = data[i] * Mint(i);
        return result;
    }

    // 積分: integral() -> 定数項0の不定積分。結果長size()+1。
    // 前提: size() < 2^23。時間・追加領域O(size()+1)。
    FPS integral() const {
        assert(size() < max_size);
        FPS result(size() + 1);
        if (empty()) return result;

        // 整数の逆元を線形時間で一括計算する。
        vector<Mint> inverse(size() + 1, Mint(0));
        inverse[1] = Mint(1);
        for (int i = 2; i <= size(); i++) {
            inverse[i] = -Mint(Mint::mod() / i) * inverse[Mint::mod() % i];
        }
        for (int i = 0; i < size(); i++) result.data[i + 1] = data[i] * inverse[i + 1];
        return result;
    }

    // 逆元: inv(n) -> 1/f mod x^nを正確にn項返す。
    // 前提: 0 <= n <= 2^22、n>0なら定数項が非零。時間O(n log n)、追加領域O(n)。
    FPS inv(int n) const {
        check_precision(n);
        if (n == 0) return FPS();
        assert(!empty() && data[0] != Mint(0));
        FPS result{data[0].inv()};

        // g <- g(2-fg)により、正しい係数の個数を倍増する。
        while (result.size() < n) {
            int length = min(result.size() * 2, n);
            FPS correction = -(pre(length) * result).pre(length);
            correction.data[0] += Mint(2);
            result = (result * correction).pre(length);
        }
        return result;
    }

    // 対数: log(n) -> log(f) mod x^nを正確にn項返す。
    // 前提: 0 <= n <= 2^22、n>0なら定数項が1。時間O(n log n)、追加領域O(n)。
    FPS log(int n) const {
        check_precision(n);
        if (n == 0) return FPS();
        assert(!empty() && data[0] == Mint(1));
        if (n == 1) return FPS(1);
        // (log f)' = f'/f。必要な次数までで計算する。
        return (pre(n).diff() * inv(n - 1)).pre(n - 1).integral();
    }

    // 指数: exp(n) -> exp(f) mod x^nを正確にn項返す。
    // 前提: 0 <= n <= 2^22、n>0なら定数項が0 (空入力も可)。時間O(n log n)、追加領域O(n)。
    FPS exp(int n) const {
        check_precision(n);
        if (n == 0) return FPS();
        assert(empty() || data[0] == Mint(0));
        FPS result{Mint(1)};

        // g <- g(1+f-log g)により、正しい係数の個数を倍増する。
        while (result.size() < n) {
            int length = min(result.size() * 2, n);
            FPS correction = pre(length) - result.log(length);
            correction.data[0] += Mint(1);
            result = (result * correction).pre(length);
        }
        return result;
    }

    // 非負整数乗: pow(k,n) -> f^k mod x^nを正確にn項返す。
    // 前提: k>=0、0 <= n <= 2^22。0^0も1とし、n=0なら空。
    // 時間O(n log n + log(k+1))、追加領域O(n)。
    FPS pow(long long k, int n) const {
        assert(k >= 0);
        check_precision(n);
        if (n == 0) return FPS();
        if (k == 0) {
            FPS result(n);
            result.data[0] = Mint(1);
            return result;
        }

        // f = x^v * c * h、h[0]=1に分ける。v*kの乗算前に打ち切りを判定する。
        int v = 0;
        while (v < min(size(), n) && data[v] == Mint(0)) v++;
        if (v == min(size(), n)) return FPS(n);
        if (v > 0 && k > (n - 1) / v) return FPS(n);
        int shift = (int)(v * k);
        int length = n - shift;
        Mint constant = data[v];
        Mint inverse_constant = constant.inv();
        FPS normalized(length);
        for (int i = 0; i < length && i < size() - v; i++) {
            normalized.data[i] = data[v + i] * inverse_constant;
        }
        FPS powered = (normalized.log(length) * Mint(k)).exp(length);
        powered *= constant.pow(k);
        FPS result(n);
        copy(powered.data.begin(), powered.data.end(), result.data.begin() + shift);
        return result;
    }
};
