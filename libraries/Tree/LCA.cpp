#include <bits/stdc++.h>
using namespace std;

// 重みなしの木のLCA: 前計算・メモリ O(n log n)、照会 O(log n)、depth は O(1)
struct LCA {
private:
    int n;
    int edge_count;
    bool built;
    vector<vector<int>> graph;
    vector<vector<int>> table;
    vector<int> depths;

public:
    // 初期化: LCA(n) -> n 頂点の空グラフ (前提: n >= 1、頂点は 0-index)
    LCA(int vertex_count) : n(vertex_count), edge_count(0), built(false) {
        assert(n >= 1);
        graph.resize(n);
    }

    // 無向辺追加: add_edge(u, v) -> 返り値なし (前提: 頂点が有効、追加後は再 build が必要)
    void add_edge(int u, int v) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        graph[u].push_back(v);
        graph[v].push_back(u);
        edge_count++;
        built = false;
    }

    // 前計算: build(root) -> 指定した根で構築し直す、返り値なし (前提: グラフは木、0 <= root < n)
    void build(int root = 0) {
        assert(0 <= root && root < n);
        assert(edge_count == n - 1);
        built = false;
        int log = 1;
        while ((1LL << log) < n) log++;
        table.assign(log, vector<int>(n, -1));
        depths.assign(n, -1);

        // 非再帰の幅優先探索で親と深さを求める
        vector<int> order;
        order.reserve(n);
        order.push_back(root);
        depths[root] = 0;
        for (int i = 0; i < (int)order.size(); i++) {
            int v = order[i];
            for (int to : graph[v]) {
                if (depths[to] != -1) continue;
                depths[to] = depths[v] + 1;
                table[0][to] = v;
                order.push_back(to);
            }
        }
        assert((int)order.size() == n);

        // table[j][v]: v の 2^j 個上の祖先 (根を超える場合は -1)
        for (int j = 1; j < log; j++) {
            for (int v = 0; v < n; v++) {
                int parent = table[j - 1][v];
                if (parent != -1) table[j][v] = table[j - 1][parent];
            }
        }
        built = true;
    }

    // 最小共通祖先: lca(u, v) -> 2頂点の最小共通祖先 (前提: build 済み、頂点が有効)
    int lca(int u, int v) const {
        assert(built && 0 <= u && u < n && 0 <= v && v < n);
        if (depths[u] < depths[v]) swap(u, v);
        u = ancestor(u, depths[u] - depths[v]);
        if (u == v) return u;

        // 共通祖先の直下まで、深さを揃えたまま同時に上る
        for (int j = (int)table.size() - 1; j >= 0; j--) {
            if (table[j][u] == table[j][v]) continue;
            u = table[j][u];
            v = table[j][v];
        }
        return table[0][u];
    }

    // 距離: dist(u, v) -> 2頂点間の辺数 (前提: build 済み、頂点が有効)
    int dist(int u, int v) const {
        int w = lca(u, v);
        return (depths[u] - depths[w]) + (depths[v] - depths[w]);
    }

    // 深さ: depth(v) -> 根から v までの辺数 (前提: build 済み、0 <= v < n)
    int depth(int v) const {
        assert(built && 0 <= v && v < n);
        return depths[v];
    }

    // 祖先: ancestor(v, k) -> k 個上の祖先 (k = 0 なら v、根を超える場合は -1)
    // 前提: build 済み、0 <= v < n、k >= 0
    int ancestor(int v, int k) const {
        assert(built && 0 <= v && v < n && k >= 0);
        if (k > depths[v]) return -1;
        for (int j = 0; k > 0; j++, k >>= 1) {
            if (k & 1) v = table[j][v];
        }
        return v;
    }
};
