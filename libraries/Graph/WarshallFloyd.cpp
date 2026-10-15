#include <bits/stdc++.h>
using namespace std;

template<class T = long long>
struct WarshallFloyd {
    static_assert(is_arithmetic_v<T> && is_signed_v<T>, "T must be a signed numeric type");

public:
    struct Edge {
        int from;
        int to;
        T cost;
        int id;
    };

    static constexpr T inf = numeric_limits<T>::max() / 4;
    static constexpr T neg_inf = -inf;

private:
    int n;
    int edge_count;
    bool built;
    bool negative_cycle;
    vector<Edge> arcs;
    vector<vector<T>> distance;
    vector<vector<int>> next_arc;

    // 距離の加算を表現可能範囲内に制限する
    static T add_cost(T a, T b) {
        if (a == neg_inf || b == neg_inf) return neg_inf;
        if (b > T(0) && a >= inf - b) return inf;
        if (b < T(0) && a <= neg_inf - b) return neg_inf;
        return a + b;
    }

    // 復元可能な有限最短経路が存在するか判定する
    bool has_finite_path(int from, int to) const {
        return distance[from][to] != inf && distance[from][to] != neg_inf;
    }

public:
    // 初期化: WarshallFloyd(n) -> n 頂点の空グラフ (前提: n >= 0)
    WarshallFloyd(int vertex_count) : n(vertex_count), edge_count(0), built(false), negative_cycle(false) {
        assert(vertex_count >= 0);
    }

    // 有向辺追加: add_edge(from, to, cost) -> 新しい辺ID (前提: 有効な頂点、neg_inf < cost < inf)
    int add_edge(int from, int to, T cost) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        assert(neg_inf < cost && cost < inf);
        int id = edge_count++;
        arcs.push_back({from, to, cost, id});
        built = false;
        return id;
    }

    // 無向辺追加: add_undirected_edge(u, v, cost) -> 両方向で共通の辺ID (前提: 有効な頂点、neg_inf < cost < inf)
    int add_undirected_edge(int u, int v, T cost) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        assert(neg_inf < cost && cost < inf);
        int id = edge_count++;
        arcs.push_back({u, v, cost, id});
        arcs.push_back({v, u, cost, id});
        built = false;
        return id;
    }

    // 全点対最短距離: build() -> 負閉路がなければ true (前提: 有限な途中距離が (-inf, inf) に収まる)
    bool build() {
        built = false;
        distance.assign(n, vector<T>(n, inf));
        next_arc.assign(n, vector<int>(n, -1));
        negative_cycle = false;
        for (int i = 0; i < n; i++) distance[i][i] = T(0);

        // 平行辺は最小コストの辺を選び、復元用の辺番号も記録する
        for (int i = 0; i < (int)arcs.size(); i++) {
            const Edge& e = arcs[i];
            if (e.cost >= distance[e.from][e.to]) continue;
            distance[e.from][e.to] = e.cost;
            next_arc[e.from][e.to] = i;
        }

        // 中継点を順番に追加して最短距離と経路情報を更新する
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                if (distance[i][k] == inf) continue;
                for (int j = 0; j < n; j++) {
                    if (distance[k][j] == inf) continue;
                    T d = add_cost(distance[i][k], distance[k][j]);
                    if (d >= distance[i][j]) continue;
                    distance[i][j] = d;
                    next_arc[i][j] = (i == k ? next_arc[k][j] : next_arc[i][k]);
                }
            }
        }

        // 負閉路を通れる頂点対だけを -inf とし、経路復元を無効化する
        for (int k = 0; k < n; k++) {
            if (distance[k][k] >= T(0)) continue;
            negative_cycle = true;
            for (int i = 0; i < n; i++) {
                if (distance[i][k] == inf) continue;
                for (int j = 0; j < n; j++) {
                    if (distance[k][j] == inf) continue;
                    distance[i][j] = neg_inf;
                    next_arc[i][j] = -1;
                }
            }
        }
        built = true;
        return !negative_cycle;
    }

    // 最短距離: dist(u, v) -> 距離、到達不能なら inf、負閉路の影響下なら neg_inf (前提: build 済み、有効な頂点)
    T dist(int u, int v) const {
        assert(built && 0 <= u && u < n && 0 <= v && v < n);
        return distance[u][v];
    }

    // 全最短距離: distances() -> 距離行列への参照 (前提: build 済み、再計算・辺追加後は再取得)
    const vector<vector<T>>& distances() const {
        assert(built);
        return distance;
    }

    // 到達判定: reachable(u, v) -> u から v に到達できれば true (前提: build 済み、有効な頂点)
    bool reachable(int u, int v) const {
        assert(built && 0 <= u && u < n && 0 <= v && v < n);
        return distance[u][v] != inf;
    }

    // 負閉路判定: has_negative_cycle() -> グラフに負閉路があれば true (前提: build 済み)
    bool has_negative_cycle() const {
        assert(built);
        return negative_cycle;
    }

    // 頂点列復元: path(u, v) -> 最短経路の頂点列 (到達不能・負閉路の影響下なら空、前提: build 済み、有効な頂点)
    vector<int> path(int u, int v) const {
        assert(built && 0 <= u && u < n && 0 <= v && v < n);
        if (!has_finite_path(u, v)) return {};
        vector<int> result = {u};
        int cur = u;
        for (int step = 0; cur != v; step++) {
            assert(step < n && next_arc[cur][v] != -1);
            cur = arcs[next_arc[cur][v]].to;
            result.push_back(cur);
        }
        return result;
    }

    // 辺列復元: edge_path(u, v) -> 最短経路の辺列 (到達不能・負閉路の影響下・u == v なら空、前提: build 済み、有効な頂点)
    vector<Edge> edge_path(int u, int v) const {
        assert(built && 0 <= u && u < n && 0 <= v && v < n);
        if (!has_finite_path(u, v)) return {};
        vector<Edge> result;
        int cur = u;
        for (int step = 0; cur != v; step++) {
            assert(step < n && next_arc[cur][v] != -1);
            const Edge& e = arcs[next_arc[cur][v]];
            result.push_back(e);
            cur = e.to;
        }
        return result;
    }
};
