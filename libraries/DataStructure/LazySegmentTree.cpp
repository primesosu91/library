#include <bits/stdc++.h>
using namespace std;

template<class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
struct LazySegmentTree {
private:
    int n;
    int size;
    int log;
    vector<S> data;
    vector<F> lazy;

    // 子ノードを結合して親ノードを更新
    void update(int k) {
        data[k] = op(data[2 * k], data[2 * k + 1]);
    }

    // ノードに f を適用し、内部ノードの未伝播操作 g を composition(f, g) で更新 (g の後に f を適用)
    void all_apply(int k, F f) {
        data[k] = mapping(f, data[k]);
        if (k < size) lazy[k] = composition(f, lazy[k]);
    }

    // 遅延操作を左右の子ノードへ伝播
    void push(int k) {
        all_apply(2 * k, lazy[k]);
        all_apply(2 * k + 1, lazy[k]);
        lazy[k] = id();
    }

public:
    // 初期化: LazySegmentTree() -> 長さ0の配列を構築
    LazySegmentTree() : LazySegmentTree(0) {}

    // 初期化: LazySegmentTree(要素数) -> 全要素を e() に設定 (前提: 要素数 >= 0)
    // op は結合則を満たし e() は単位元、mapping は op と両立し、composition は作用の合成、id() は恒等操作
    // 区間和で S に区間長を持たせる場合、e() の長さは 0 なので、長さ 1 の要素を初期値とするには配列から構築する
    LazySegmentTree(int element_count) : n(element_count), size(1), log(0) {
        assert(element_count >= 0);

        while (size < n) {
            size *= 2;
            log++;
        }
        data.assign(2 * size, e());
        lazy.assign(size, id());
    }

    // 初期化: LazySegmentTree(初期配列) -> 配列の値から構築
    // 使用例 (区間加算・区間和): S={sum,len}、F=long long、op は成分ごとの和、e()={0,0}
    // mapping(f,x)={x.sum+f*x.len,x.len}、composition(f,g)=f+g、id()=0
    // vector<S> v(n, S{0,1}); LazySegmentTree<S,op,e,F,mapping,composition,id> seg(v); seg.apply(0,n,5); seg.prod(0,n).sum;
    LazySegmentTree(const vector<S>& v) : LazySegmentTree((int)v.size()) {
        // 葉に初期値を配置し、下から親を構築
        for (int i = 0; i < n; i++) {
            data[size + i] = v[i];
        }
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    // 点更新: set(位置 p, 値 x) -> a[p] を x に変更（0 <= p < n）
    void set(int p, S x) {
        assert(0 <= p && p < n);

        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        data[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    // 点取得: get(位置 p) -> a[p] を返す（0 <= p < n）
    S get(int p) {
        assert(0 <= p && p < n);

        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return data[p];
    }

    // 区間取得: prod(左端 l, 右端 r) -> [l, r) の演算結果（0 <= l <= r <= n）
    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return e();

        l += size;
        r += size;
        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        S left_value = e();
        S right_value = e();
        // 非可換演算でも結合順序を維持
        while (l < r) {
            if (l & 1) left_value = op(left_value, data[l++]);
            if (r & 1) right_value = op(data[--r], right_value);
            l >>= 1;
            r >>= 1;
        }
        return op(left_value, right_value);
    }

    // 全区間取得: all_prod() -> [0, n) の演算結果を返す
    S all_prod() const {
        return data[1];
    }

    // 点作用: apply(位置 p, 操作 f) -> a[p] に f を適用（0 <= p < n）
    void apply(int p, F f) {
        assert(0 <= p && p < n);

        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        data[p] = mapping(f, data[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    // 区間作用: apply(左端 l, 右端 r, 操作 f) -> [l, r) に f を適用（0 <= l <= r <= n）
    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return;

        l += size;
        r += size;
        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        int left = l;
        int right = r;
        // 対象ノードに一括作用し、必要な祖先のみ更新
        while (l < r) {
            if (l & 1) all_apply(l++, f);
            if (r & 1) all_apply(--r, f);
            l >>= 1;
            r >>= 1;
        }
        l = left;
        r = right;
        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    // 右方向の二分探索: max_right(左端 l, 判定関数 g) -> g(prod(l, r)) が true の最大の r
    // 前提: 0 <= l <= n、g(e()) == true、g に副作用がなく判定が単調
    template<class G>
    int max_right(int l, G g) {
        assert(0 <= l && l <= n);
        assert(g(e()));
        if (l == n) return n;

        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        S value = e();
        do {
            while ((l & 1) == 0) l >>= 1;
            if (!g(op(value, data[l]))) {
                // 条件が初めて破れる葉まで遅延操作を伝播しながら降りる
                while (l < size) {
                    push(l);
                    l *= 2;
                    if (g(op(value, data[l]))) {
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

    // 左方向の二分探索: min_left(右端 r, 判定関数 g) -> g(prod(l, r)) が true の最小の l
    // 前提: 0 <= r <= n、g(e()) == true、g に副作用がなく判定が単調
    template<class G>
    int min_left(int r, G g) {
        assert(0 <= r && r <= n);
        assert(g(e()));
        if (r == 0) return 0;

        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        S value = e();
        do {
            r--;
            while (r > 1 && (r & 1)) r >>= 1;
            if (!g(op(data[r], value))) {
                // 条件が初めて破れる葉まで遅延操作を伝播しながら降りる
                while (r < size) {
                    push(r);
                    r = 2 * r + 1;
                    if (g(op(data[r], value))) {
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
