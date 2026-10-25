#include <bits/stdc++.h>
using namespace std;

template<class T>
struct FenwickTree {
private:
    int n;
    vector<T> data;

public:
    // 初期化: FenwickTree(要素数) -> 全要素を 0 に設定（前提: 要素数 >= 0）
    FenwickTree(int element_count) : n(element_count) {
        assert(element_count >= 0);
        data.assign(n + 1, T(0));
    }

    // 初期化: FenwickTree(初期配列) -> 配列の値から O(n) で構築
    FenwickTree(const vector<T>& v) : FenwickTree((int)v.size()) {
        for (int i = 1; i <= n; i++) {
            data[i] += v[i - 1];
            int j = i + (i & -i);
            if (j <= n) data[j] += data[i];
        }
    }

    // 点加算: add(位置 p, 値 x) -> a[p] に x を加算（0 <= p < n）
    void add(int p, T x) {
        assert(0 <= p && p < n);
        for (int i = p + 1; i <= n; i += i & -i) {
            data[i] += x;
        }
    }

    // 累積和: sum(右端 r) -> [0, r) の和（0 <= r <= n）
    T sum(int r) const {
        assert(0 <= r && r <= n);
        T result = T(0);
        for (int i = r; i > 0; i -= i & -i) {
            result += data[i];
        }
        return result;
    }

    // 区間和: sum(左端 l, 右端 r) -> [l, r) の和（0 <= l <= r <= n）
    T sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        return sum(r) - sum(l);
    }

    // 点取得: get(位置 p) -> a[p]（0 <= p < n）
    T get(int p) const {
        assert(0 <= p && p < n);
        return sum(p, p + 1);
    }

    // 点代入: set(位置 p, 値 x) -> a[p] を x に変更（0 <= p < n）
    void set(int p, T x) {
        assert(0 <= p && p < n);
        add(p, x - get(p));
    }

    // 累積和探索: lower_bound(w) -> sum(p + 1) >= w を満たす最小の p、なければ n
    // 前提: 全要素が非負で T に大小比較が定義される。w <= 0 のときは 0 を返す
    int lower_bound(T w) const {
        if (w <= T(0)) return 0;

        int p = 0;
        T value = T(0);
        int step = 1;
        while (step <= n / 2) step <<= 1;

        // 和が w 未満に収まる最長の接頭辞を探索
        for (; step > 0; step >>= 1) {
            if (p + step <= n && value + data[p + step] < w) {
                value += data[p + step];
                p += step;
            }
        }
        return p;
    }
};
