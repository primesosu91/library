#include <bits/stdc++.h>
using namespace std;

template<class T = long long>
struct Dijkstra {
    static_assert(is_arithmetic_v<T> && !is_same_v<T, bool>, "T must be a numeric type");

public:
    struct Edge {
        int from;
        int to;
        T cost;
        int id;
    };

    static constexpr T inf = numeric_limits<T>::max() / 4;

private:
    int n;
    int edge_count;
    int source;
    vector<vector<Edge>> graph;
    vector<T> distance;
    vector<Edge> parent_edge;

    // 指定した辺IDを持つ有向辺を内部に格納
    void add_arc(int from, int to, T cost, int id) {
        graph[from].push_back({from, to, cost, id});
    }

public:
    // 初期化: Dijkstra(頂点数 n) -> n 頂点の空グラフ (前提: n >= 0)
    Dijkstra(int vertex_count) : n(vertex_count), edge_count(0), source(-1) {
        assert(vertex_count >= 0);
        graph.resize(n);
    }

    // 有向辺追加: add_edge(from, to, cost) -> 新しい辺ID (前提: 頂点が有効、0 <= cost < inf)
    int add_edge(int from, int to, T cost) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        assert(T(0) <= cost && cost < inf);
        int id = edge_count++;
        add_arc(from, to, cost, id);
        source = -1;
        return id;
    }

    // 無向辺追加: add_undirected_edge(u, v, cost) -> 両方向で共通の辺ID (前提: 頂点が有効、0 <= cost < inf)
    int add_undirected_edge(int u, int v, T cost) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        assert(T(0) <= cost && cost < inf);
        int id = edge_count++;
        add_arc(u, v, cost, id);
        add_arc(v, u, cost, id);
        source = -1;
        return id;
    }

    // 最短路計算: build(s) -> 始点 s から全頂点への最短距離と復元情報を構築 (前提: 0 <= s < n)
    void build(int s) {
        assert(0 <= s && s < n);
        source = s;
        distance.assign(n, inf);
        parent_edge.assign(n, {-1, -1, T(0), -1});
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
        distance[s] = T(0);
        pq.push({T(0), s});

        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();
            if (distance[v] != d) continue;

            for (const Edge& e : graph[v]) {
                // inf 以上の経路は到達不能として扱い、加算のオーバーフローも防ぐ
                if (e.cost >= inf - d) continue;
                T next_dist = d + e.cost;
                if (next_dist >= distance[e.to]) continue;
                distance[e.to] = next_dist;
                parent_edge[e.to] = e;
                pq.push({next_dist, e.to});
            }
        }
    }

    // 最短距離: dist(v) -> 始点から v への距離 (到達不能なら inf、前提: build 済み、0 <= v < n)
    T dist(int v) const {
        assert(source != -1 && 0 <= v && v < n);
        return distance[v];
    }

    // 全最短距離: distances() -> 頂点順の距離配列への参照 (前提: build 済み、再計算や辺追加後は再取得)
    const vector<T>& distances() const {
        assert(source != -1);
        return distance;
    }

    // 到達判定: reachable(v) -> 始点から v に到達可能なら true (前提: build 済み、0 <= v < n)
    bool reachable(int v) const {
        assert(source != -1 && 0 <= v && v < n);
        return distance[v] != inf;
    }

    // 頂点列復元: path(v) -> 始点から v までの頂点列 (到達不能なら空、前提: build 済み、0 <= v < n)
    vector<int> path(int v) const {
        assert(source != -1 && 0 <= v && v < n);
        if (!reachable(v)) return {};
        vector<int> result;
        for (int cur = v; cur != source; cur = parent_edge[cur].from) {
            result.push_back(cur);
        }
        result.push_back(source);
        reverse(result.begin(), result.end());
        return result;
    }

    // 辺列復元: edge_path(v) -> 始点から v までの辺列 (到達不能・始点自身なら空、前提: build 済み、0 <= v < n)
    vector<Edge> edge_path(int v) const {
        assert(source != -1 && 0 <= v && v < n);
        if (!reachable(v)) return {};
        vector<Edge> result;
        for (int cur = v; cur != source; cur = parent_edge[cur].from) {
            result.push_back(parent_edge[cur]);
        }
        reverse(result.begin(), result.end());
        return result;
    }
};
