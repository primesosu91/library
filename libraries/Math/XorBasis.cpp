#include <bits/stdc++.h>
using namespace std;

// 非負整数を64ビットのベクトルとして扱うXOR基底。空集合のXORは0。
// W = 64として、rank()はO(1)、values()以外の操作はO(W)、内部領域はO(W)。
// 元要素の選択復元・追加回数・部分集合の選び方の数は保持しない。
struct XorBasis {
private:
    array<unsigned long long, 64> pivots;
    int dimension;

public:
    // 初期化: XorBasis() -> 空の基底を構築。
    XorBasis() : pivots{}, dimension(0) {}

    // 値追加: add(x) -> xが独立なら追加してtrue、既存の基底で表現可能ならfalse。
    bool add(unsigned long long x) {
        // 最高位ビットを既存の基底で消去し、空いている位置に登録する。
        for (int i = 63; i >= 0; i--) {
            if (((x >> i) & 1ULL) == 0) continue;
            if (pivots[i] != 0) {
                x ^= pivots[i];
            } else {
                pivots[i] = x;
                dimension++;
                return true;
            }
        }
        return false;
    }

    // ランク取得: rank() -> 独立な基底ベクトルの個数 (0〜64)。
    int rank() const { return dimension; }

    // 表現可能判定: contains(x) -> 追加した値の部分集合XORでxを作れるか。0は常にtrue。
    bool contains(unsigned long long x) const {
        return min_xor(x) == 0;
    }

    // 最大値: max_xor(x=0) -> 作れるXOR値vに対するx ^ vの最大値。
    // x自体が表現可能である必要はなく、空の基底ではxを返す。
    unsigned long long max_xor(unsigned long long x = 0) const {
        // 上位ビットから、値が大きくなる場合だけ基底を適用する。
        for (int i = 63; i >= 0; i--) x = max(x, x ^ pivots[i]);
        return x;
    }

    // 最小値: min_xor(x=0) -> 作れるXOR値vに対するx ^ vの最小値。
    // x自体が表現可能である必要はなく、空の基底ではxを返す。
    unsigned long long min_xor(unsigned long long x = 0) const {
        // 上位ビットから、値が小さくなる場合だけ基底を適用する。
        for (int i = 63; i >= 0; i--) x = min(x, x ^ pivots[i]);
        return x;
    }

    // 基底取得: basis() -> 最高位ビットの降順で独立な基底ベクトルのコピーを返す。
    // 数値のソート順・元要素との一致は保証しない。空の基底では空配列を返す。
    vector<unsigned long long> basis() const {
        vector<unsigned long long> result;
        result.reserve(dimension);
        for (int i = 63; i >= 0; i--) {
            if (pivots[i] != 0) result.push_back(pivots[i]);
        }
        return result;
    }

    // 全列挙: values() -> 作れる部分集合XORを重複なく返す。順序保証なし、空の基底では{0}。
    // 前提: 2^rank()がvectorのmax_size()以下で、列挙に十分なメモリ・時間があること。
    // 時間O(W + 2^rank())、返り値を含む追加領域O(2^rank()) (W = 64)。
    vector<unsigned long long> values() const {
        vector<unsigned long long> result;
        size_t count = 1;
        // シフトを使わず、必要な要素数をオーバーフロー前に検査する。
        for (int i = 0; i < dimension; i++) {
            assert(count <= result.max_size() / 2);
            count *= 2;
        }
        result.reserve(count);
        result.push_back(0);
        // 独立な基底を1つ加えるごとに、作れるXOR値は重複なく倍になる。
        for (int i = 63; i >= 0; i--) {
            if (pivots[i] == 0) continue;
            size_t size = result.size();
            for (size_t j = 0; j < size; j++) result.push_back(result[j] ^ pivots[i]);
        }
        return result;
    }
};
