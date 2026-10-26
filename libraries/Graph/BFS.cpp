#include <bits/stdc++.h>
using namespace std;

struct BFS {
public:
    struct Edge {
        int from;
        int to;
        int id;
    };

    static constexpr int inf = numeric_limits<int>::max() / 4;

private:
    int n;
    int edge_count;
    bool built;
    vector<vector<Edge>> graph;
    vector<int> distance;
    vector<Edge> parent_edge;

    // 指定した辺IDを持つ有向辺を内部に格納
    void add_arc(int from, int to, int id) {
        graph[from].push_back({from, to, id});
    }

public:
    // 初期化: BFS(n) -> n 頂点の空グラフ (前提: n >= 0)
    BFS(int vertex_count) : n(vertex_count), edge_count(0), built(false) {
        assert(vertex_count >= 0);
        graph.resize(n);
    }

    // 有向辺追加: add_edge(from, to) -> 新しい辺ID (前提: 頂点が有効)
    int add_edge(int from, int to) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int id = edge_count++;
        add_arc(from, to, id);
        built = false;
        return id;
    }

    // 無向辺追加: add_undirected_edge(u, v) -> 両方向で共通の辺ID (前提: 頂点が有効)
    int add_undirected_edge(int u, int v) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        int id = edge_count++;
        add_arc(u, v, id);
        add_arc(v, u, id);
        built = false;
        return id;
    }

    // 単一始点最短路: build(s) -> 始点 s から全頂点への最短距離と復元情報を構築 (前提: 0 <= s < n)
    void build(int s) {
        assert(0 <= s && s < n);
        build(vector<int>{s});
    }

    // 多点始点最短路: build(sources) -> 始点集合からの最短距離と復元情報を構築 (前提: 全始点が有効、空集合も可)
    void build(const vector<int>& sources) {
        built = false;
        distance.assign(n, inf);
        parent_edge.assign(n, {-1, -1, -1});
        queue<int> que;

        // 重複する始点は一度だけ登録する
        for (int s : sources) {
            assert(0 <= s && s < n);
            if (distance[s] == 0) continue;
            distance[s] = 0;
            que.push(s);
        }

        // 初めて訪問したときの辺を記録し、最短路を確定する
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (const Edge& e : graph[v]) {
                if (distance[e.to] != inf) continue;
                distance[e.to] = distance[v] + 1;
                parent_edge[e.to] = e;
                que.push(e.to);
            }
        }
        built = true;
    }

    // 最短距離: dist(v) -> 始点集合から v への距離 (到達不能なら inf、前提: build 済み、0 <= v < n)
    int dist(int v) const {
        assert(built && 0 <= v && v < n);
        return distance[v];
    }

    // 全最短距離: distances() -> 頂点順の距離配列への参照 (前提: build 済み、再計算や辺追加後は再取得)
    const vector<int>& distances() const {
        assert(built);
        return distance;
    }

    // 到達判定: reachable(v) -> 始点集合から v に到達できれば true (前提: build 済み、0 <= v < n)
    bool reachable(int v) const {
        assert(built && 0 <= v && v < n);
        return distance[v] != inf;
    }

    // 頂点列復元: path(v) -> いずれかの始点から v までの頂点列 (到達不能なら空、前提: build 済み、0 <= v < n)
    vector<int> path(int v) const {
        assert(built && 0 <= v && v < n);
        if (!reachable(v)) return {};
        vector<int> result;
        for (int cur = v; cur != -1; cur = parent_edge[cur].from) {
            result.push_back(cur);
        }
        reverse(result.begin(), result.end());
        return result;
    }

    // 辺列復元: edge_path(v) -> 最短経路の辺列 (到達不能・始点自身なら空、前提: build 済み、0 <= v < n)
    vector<Edge> edge_path(int v) const {
        assert(built && 0 <= v && v < n);
        if (!reachable(v)) return {};
        vector<Edge> result;
        for (int cur = v; parent_edge[cur].from != -1; cur = parent_edge[cur].from) {
            result.push_back(parent_edge[cur]);
        }
        reverse(result.begin(), result.end());
        return result;
    }
};
