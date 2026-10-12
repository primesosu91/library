#include <bits/stdc++.h>
using namespace std;

template<class S, S (*op)(S, S), S (*e)()>
struct SegmentTree {
private:
    int n;
    int size;
    vector<S> data;

    // 親ノードの値を更新
    void update(int k) {
        data[k] = op(data[2 * k], data[2 * k + 1]);
    }

public:
    // 初期化: SegmentTree(要素数) -> 全要素を e() に設定
    SegmentTree(int element_count) : n(element_count), size(1) {
        assert(element_count >= 0);

        while (size < n) size *= 2;
        data.assign(2 * size, e());
    }

    // 初期化: SegmentTree(初期配列) -> 配列の値を使って構築
    SegmentTree(const vector<S>& v) : SegmentTree((int)v.size()) {
        // 葉に初期値を配置し、下から親を構築
        for (int i = 0; i < n; i++) {
            data[size + i] = v[i];
        }
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    // 点更新: set(位置 p, 値 x) -> a[p] を x に変更
    void set(int p, S x) {
        assert(0 <= p && p < n);

        p += size;
        data[p] = x;
        while (p > 1) {
            p /= 2;
            update(p);
        }
    }

    // 点取得: get(位置 p) -> a[p]
    S get(int p) const {
        assert(0 <= p && p < n);
        return data[size + p];
    }

    // 区間取得: prod(左端 l, 右端 r) -> [l, r) の演算結果
    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);

        S left_value = e();
        S right_value = e();
        l += size;
        r += size;

        // 左右の結果を分け、非可換演算でも順序を保つ
        while (l < r) {
            if (l & 1) left_value = op(left_value, data[l++]);
            if (r & 1) right_value = op(data[--r], right_value);
            l /= 2;
            r /= 2;
        }
        return op(left_value, right_value);
    }

    // 全区間取得: all_prod() -> [0, n) の演算結果
    S all_prod() const {
        return data[1];
    }

    // 右方向の二分探索: max_right(左端 l, 判定関数 f) -> f(prod(l, r)) が true となる最大の r
    // 前提: f(e()) == true、f は区間を右へ伸ばすと true から false にのみ変化する
    template<class F>
    int max_right(int l, F f) const {
        assert(0 <= l && l <= n);
        assert(f(e()));
        if (l == n) return n;

        l += size;
        S value = e();
        do {
            while ((l & 1) == 0) l /= 2;
            if (!f(op(value, data[l]))) {
                // 条件が初めて破れる葉まで降りる
                while (l < size) {
                    l *= 2;
                    if (f(op(value, data[l]))) {
                        value = op(value, data[l]);
                        l++;
                    }
                }
                return l - size;
            }
            value = op(value, data[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    // 左方向の二分探索: min_left(右端 r, 判定関数 f) -> f(prod(l, r)) が true となる最小の l
    // 前提: f(e()) == true、f は区間を左へ伸ばすと true から false にのみ変化する
    template<class F>
    int min_left(int r, F f) const {
        assert(0 <= r && r <= n);
        assert(f(e()));
        if (r == 0) return 0;

        r += size;
        S value = e();
        do {
            r--;
            while (r > 1 && (r & 1)) r /= 2;
            if (!f(op(data[r], value))) {
                // 条件が初めて破れる葉まで降りる
                while (r < size) {
                    r = 2 * r + 1;
                    if (f(op(data[r], value))) {
                        value = op(data[r], value);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            value = op(data[r], value);
        } while ((r & -r) != r);
        return 0;
    }
};
