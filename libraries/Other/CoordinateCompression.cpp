#include <bits/stdc++.h>
using namespace std;

template<class T>
struct CoordinateCompression {
private:
    vector<T> data;

public:
    // 初期化: CoordinateCompression(初期配列 v) -> 昇順・重複なしの座標表を O(n log n) で構築
    // 前提: T に大小比較・等値比較が定義される
    CoordinateCompression(const vector<T>& v) : data(v) {
        sort(data.begin(), data.end());
        data.erase(unique(data.begin(), data.end()), data.end());
    }

    // 座標数: size() -> 異なる登録値の個数
    int size() const {
        return (int)data.size();
    }

    // 圧縮: get(値 x) -> x の圧縮後の座標（前提: x は登録済み）
    int get(const T& x) const {
        int i = lower_bound(x);
        assert(i < size() && data[i] == x);
        return i;
    }

    // 復元: value(座標 i) -> 対応する元の値（0 <= i < size()）
    const T& value(int i) const {
        assert(0 <= i && i < size());
        return data[i];
    }

    // 配列圧縮: compress(配列 v) -> 各値の圧縮後の座標（前提: 全要素が登録済み）
    vector<int> compress(const vector<T>& v) const {
        vector<int> result;
        result.reserve(v.size());
        for (const T& x : v) result.push_back(get(x));
        return result;
    }

    // 下限位置: lower_bound(値 x) -> x 未満の登録値の個数（x は未登録でもよい）
    int lower_bound(const T& x) const {
        return (int)(std::lower_bound(data.begin(), data.end(), x) - data.begin());
    }

    // 上限位置: upper_bound(値 x) -> x 以下の登録値の個数（x は未登録でもよい）
    int upper_bound(const T& x) const {
        return (int)(std::upper_bound(data.begin(), data.end(), x) - data.begin());
    }
};
