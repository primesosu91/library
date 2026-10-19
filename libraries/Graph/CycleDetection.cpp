#include <bits/stdc++.h>
using namespace std;

struct CycleDetection {
public:
    struct Edge {
        int from;
        int to;
        int id;
    };

private:
    struct Arc {
        int to;
        int id;
    };

    int n;
    int edge_count;
    bool directed;
    bool built;
    bool found;
    vector<vector<Arc>> graph;
    vector<int> vertices;
    vector<Edge> edges;

public:
    // 初期化: CycleDetection(n, directed) -> n 頂点の有向/無向グラフ (前提: n >= 0)
    CycleDetection(int vertex_count, bool is_directed)
        : n(vertex_count), edge_count(0), directed(is_directed), built(false), found(false) {
        assert(vertex_count >= 0);
        graph.resize(n);
    }

    // 辺追加: add_edge(from, to) -> 新しい辺ID (前提: 頂点が有効、無向なら両方向に登録)
    int add_edge(int from, int to) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int id = edge_count++;
        graph[from].push_back({to, id});
        if (!directed) graph[to].push_back({from, id});
        built = false;
        return id;
    }

    // サイクル探索: build() -> サイクルが存在すれば true (長さ1の自己ループ、無向の平行辺も対象)
    bool build() {
        built = false;
        found = false;
        vertices.clear();
        edges.clear();
        vector<int> state(n, 0), parent(n, -1), parent_edge(n, -1), index(n, 0);
        vector<int> stack;
        stack.reserve(n);

        // 再帰を使わないDFSで探索中の祖先への辺を見つける
        for (int root = 0; root < n; root++) {
            if (state[root] != 0) continue;
            state[root] = 1;
            stack.push_back(root);
            while (!stack.empty()) {
                int v = stack.back();
                if (index[v] == (int)graph[v].size()) {
                    state[v] = 2;
                    stack.pop_back();
                    continue;
                }
                Arc a = graph[v][index[v]++];
                if (!directed && a.id == parent_edge[v]) continue;
                if (state[a.to] == 0) {
                    parent[a.to] = v;
                    parent_edge[a.to] = a.id;
                    state[a.to] = 1;
                    stack.push_back(a.to);
                } else if (state[a.to] == 1) {
                    // 祖先から現在頂点までの木辺と、祖先へ戻る辺を復元する
                    for (int cur = v; cur != a.to; cur = parent[cur]) vertices.push_back(cur);
                    vertices.push_back(a.to);
                    reverse(vertices.begin(), vertices.end());
                    for (int i = 1; i < (int)vertices.size(); i++) {
                        int u = vertices[i - 1], to = vertices[i];
                        edges.push_back({u, to, parent_edge[to]});
                    }
                    edges.push_back({v, a.to, a.id});
                    found = true;
                    built = true;
                    return true;
                }
            }
        }
        built = true;
        return false;
    }

    // 存在判定: has_cycle() -> サイクルが見つかっていれば true (前提: build 済み)
    bool has_cycle() const {
        assert(built);
        return found;
    }

    // 頂点列復元: cycle_vertices() -> 閉路上の頂点列 (閉路なしなら空、前提: build 済み)
    vector<int> cycle_vertices() const {
        assert(built);
        return vertices;
    }

    // 辺列復元: cycle_edges() -> 頂点列と同じ順の辺列 (閉路なしなら空、前提: build 済み)
    vector<Edge> cycle_edges() const {
        assert(built);
        return edges;
    }
};
