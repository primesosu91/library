#include <bits/stdc++.h>
using namespace std;

template<class T>
struct PrefixSum {
private:
    int n;
    vector<T> data;

public:
    // 初期化: PrefixSum(初期配列) -> 累積和を O(n) で構築
    // 前提: T は 0 で初期化でき、加算・減算が定義される
    PrefixSum(const vector<T>& v) : n((int)v.size()), data(n + 1, T(0)) {
        for (int i = 0; i < n; i++) {
            data[i + 1] = data[i] + v[i];
        }
    }

    // 累積和: sum(右端 r) -> [0, r) の和（0 <= r <= n）
    T sum(int r) const {
        assert(0 <= r && r <= n);
        return data[r];
    }

    // 区間和: sum(左端 l, 右端 r) -> [l, r) の和（0 <= l <= r <= n）
    T sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        return data[r] - data[l];
    }

    // 点取得: get(位置 p) -> a[p]（0 <= p < n）
    T get(int p) const {
        assert(0 <= p && p < n);
        return data[p + 1] - data[p];
    }
};
