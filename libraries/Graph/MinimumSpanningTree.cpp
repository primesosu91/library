#include <bits/stdc++.h>
using namespace std;

// Kruskal法: 構築 O(n + m log(m + 1))、メモリ O(n + m)
// T は bool 以外の整数型・浮動小数点型。浮動小数点の比較に許容誤差は設けない
template<class T = long long>
struct MinimumSpanningTree {
    static_assert(is_arithmetic_v<T> && !is_same_v<T, bool>, "T must be a numeric type");

public:
    struct Edge {
        int from;
        int to;
        T cost;
        int id;
    };

private:
    int n;
    int component_count;
    bool built;
    T total_cost;
    vector<Edge> graph_edges;
    vector<Edge> selected_edges;
    vector<int> parent_or_size;

    // 経路圧縮で代表元を取得
    int leader(int v) {
        if (parent_or_size[v] < 0) return v;
        return parent_or_size[v] = leader(parent_or_size[v]);
    }

public:
    // 初期化: MinimumSpanningTree(n) -> n 頂点の空グラフ (前提: n >= 0、頂点は 0-index)
    MinimumSpanningTree(int vertex_count)
        : n(vertex_count), component_count(vertex_count), built(false), total_cost(0) {
        assert(vertex_count >= 0);
    }

    // 無向辺追加: add_edge(u, v, cost) -> 追加順の辺ID (前提: 頂点が有効、追加後は再 build が必要)
    // 負辺・自己ループ・多重辺に対応。浮動小数点の cost は有限値 (NaN・無限大は不可)
    int add_edge(int u, int v, T cost) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        if constexpr (is_floating_point_v<T>) assert(isfinite(cost));
        int id = (int)graph_edges.size();
        graph_edges.push_back({u, v, cost, id});
        built = false;
        return id;
    }

    // 最小全域森構築: build() -> 返り値なし (前提: 加算途中を含む重みの総和が T の範囲内)
    // 浮動小数点の総和には丸め誤差が生じる
    // 非連結なら各連結成分の最小全域木を求める。同じ重みは辺ID順に処理
    void build() {
        built = false;
        total_cost = T(0);
        component_count = n;
        selected_edges.clear();
        parent_or_size.assign(n, -1);
        vector<int> order(graph_edges.size());
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            if (graph_edges[a].cost != graph_edges[b].cost) {
                return graph_edges[a].cost < graph_edges[b].cost;
            }
            return a < b;
        });

        // 異なる成分を結ぶ辺だけを採用し、小さい集合を大きい集合に統合
        for (int id : order) {
            const Edge& e = graph_edges[id];
            int u = leader(e.from), v = leader(e.to);
            if (u == v) continue;
            if (-parent_or_size[u] < -parent_or_size[v]) swap(u, v);
            parent_or_size[u] += parent_or_size[v];
            parent_or_size[v] = u;
            component_count--;
            total_cost += e.cost;
            selected_edges.push_back(e);
        }
        built = true;
    }

    // 総コスト: cost() -> 最小全域森の重みの総和 (前提: build 済み)
    T cost() const {
        assert(built);
        return total_cost;
    }

    // 採用辺列: edges() -> 採用順の辺列への参照 (前提: build 済み、辺追加・再構築後は再取得)
    const vector<Edge>& edges() const {
        assert(built);
        return selected_edges;
    }

    // 成分数: count() -> 連結成分の個数 (前提: build 済み)
    int count() const {
        assert(built);
        return component_count;
    }

    // 連結判定: is_connected() -> 連結なら true、空グラフも true (前提: build 済み)
    bool is_connected() const {
        assert(built);
        return component_count <= 1;
    }
};
