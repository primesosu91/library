#include <bits/stdc++.h>
using namespace std;

struct UnionFind {
private:
    int n;
    int component_count;
    vector<int> parent_or_size;
    vector<int> next;

public:
    // 初期化: UnionFind() -> 要素数0の集合を生成
    UnionFind() : UnionFind(0) {}

    // 初期化: UnionFind(要素数 n) -> n 個の独立した集合を生成 (前提: n >= 0)
    UnionFind(int element_count) : n(element_count), component_count(element_count) {
        assert(element_count >= 0);
        parent_or_size.assign(n, -1);
        next.resize(n);
        iota(next.begin(), next.end(), 0);
    }

    // 集合の統合: merge(a, b) -> 統合後の代表元 (前提: 0 <= a, b < n)
    int merge(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        a = leader(a);
        b = leader(b);
        if (a == b) return a;

        // 小さい集合を大きい集合に統合する
        if (-parent_or_size[a] < -parent_or_size[b]) swap(a, b);
        parent_or_size[a] += parent_or_size[b];
        parent_or_size[b] = a;

        // 各集合の循環リストを連結する
        swap(next[a], next[b]);
        component_count--;
        return a;
    }

    // 同一集合判定: same(a, b) -> 同じ集合なら true (前提: 0 <= a, b < n)
    bool same(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        return leader(a) == leader(b);
    }

    // 代表元取得: leader(a) -> a の所属する集合の代表元 (前提: 0 <= a < n)
    int leader(int a) {
        assert(0 <= a && a < n);
        if (parent_or_size[a] < 0) return a;
        return parent_or_size[a] = leader(parent_or_size[a]);
    }

    // 集合サイズ取得: size(a) -> a の所属する集合の要素数 (前提: 0 <= a < n)
    int size(int a) {
        assert(0 <= a && a < n);
        return -parent_or_size[leader(a)];
    }

    // 集合数取得: count() -> 現在の集合数
    int count() const {
        return component_count;
    }

    // 所属集合取得: group(a) -> a の所属する集合の要素一覧 (前提: 0 <= a < n、順序は不定)
    vector<int> group(int a) {
        int root = leader(a);
        vector<int> result;
        result.reserve(-parent_or_size[root]);
        int v = root;
        do {
            result.push_back(v);
            v = next[v];
        } while (v != root);
        return result;
    }

    // 全集合取得: groups() -> 各集合の要素一覧 (集合・要素の順序は不定)
    vector<vector<int>> groups() {
        vector<vector<int>> result;
        result.reserve(component_count);
        for (int root = 0; root < n; root++) {
            if (parent_or_size[root] >= 0) continue;
            vector<int> group;
            group.reserve(-parent_or_size[root]);
            int v = root;
            do {
                group.push_back(v);
                v = next[v];
            } while (v != root);
            result.push_back(move(group));
        }
        return result;
    }
};
