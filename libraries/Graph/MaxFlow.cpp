#include <bits/stdc++.h>
using namespace std;

template<class T = long long>
struct MaxFlow {
    static_assert(is_integral_v<T> && !is_same_v<T, bool>, "T must be an integer type");

public:
    struct Edge {
        int from;
        int to;
        T cap;
        T flow;
    };

private:
    struct ResidualEdge {
        int to;
        int rev;
        T cap;
    };

    int n;
    vector<vector<ResidualEdge>> graph;
    vector<pair<int, int>> pos;

    // レベルグラフを構築し、終点への到達可能性を返す
    bool bfs(int s, int t, vector<int>& level) const {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (const ResidualEdge& e : graph[v]) {
                if (e.cap == T(0) || level[e.to] != -1) continue;
                level[e.to] = level[v] + 1;
                que.push(e.to);
            }
        }
        return level[t] != -1;
    }

    // 再帰を使わず、レベルグラフ上の増加路に流せる量を求めて流す
    T augment(int s, int t, T limit, const vector<int>& level, vector<int>& iter,
              vector<int>& vertices, vector<int>& edge_indices, vector<T>& minimum) {
        vertices.clear();
        edge_indices.clear();
        minimum.clear();
        vertices.push_back(s);
        minimum.push_back(limit);

        while (!vertices.empty()) {
            int v = vertices.back();
            if (v == t) {
                T pushed = minimum.back();
                for (int i = 0; i < (int)edge_indices.size(); i++) {
                    ResidualEdge& e = graph[vertices[i]][edge_indices[i]];
                    e.cap -= pushed;
                    graph[e.to][e.rev].cap += pushed;
                }
                return pushed;
            }

            int& i = iter[v];
            while (i < (int)graph[v].size()) {
                const ResidualEdge& e = graph[v][i];
                if (e.cap > T(0) && level[e.to] == level[v] + 1) break;
                i++;
            }
            if (i == (int)graph[v].size()) {
                vertices.pop_back();
                minimum.pop_back();
                if (vertices.empty()) break;
                edge_indices.pop_back();
                iter[vertices.back()]++;
            } else {
                const ResidualEdge& e = graph[v][i];
                T next_min = min(minimum.back(), e.cap);
                edge_indices.push_back(i);
                vertices.push_back(e.to);
                minimum.push_back(next_min);
            }
        }
        return T(0);
    }

public:
    // 初期化: MaxFlow(n) -> n 頂点の空グラフ (前提: n >= 0)
    MaxFlow(int vertex_count) : n(vertex_count) {
        assert(vertex_count >= 0);
        graph.resize(n);
    }

    // 有向辺追加: add_edge(from, to, cap) -> 追加した辺ID (前提: 頂点が有効、cap >= 0)
    int add_edge(int from, int to, T cap) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        assert(cap >= T(0));
        int id = (int)pos.size();
        int from_id = (int)graph[from].size();
        int to_id = (int)graph[to].size();
        if (from == to) to_id++;
        pos.push_back({from, from_id});
        graph[from].push_back({to, to_id, cap});
        graph[to].push_back({from, from_id, T(0)});
        return id;
    }

    // 追加最大流: flow(s, t) -> 残余グラフ上で追加可能な最大流量 (前提: s, t が有効、s != t、総流量が T に収まる)
    T flow(int s, int t) {
        return flow(s, t, numeric_limits<T>::max());
    }

    // 流量制限付き追加最大流: flow(s, t, limit) -> 追加流量 (前提: s, t が有効、s != t、limit >= 0)
    T flow(int s, int t, T limit) {
        assert(0 <= s && s < n && 0 <= t && t < n && s != t);
        assert(limit >= T(0));
        T result = T(0);
        vector<int> level(n), iter(n);
        vector<int> vertices, edge_indices;
        vector<T> minimum;
        vertices.reserve(n);
        edge_indices.reserve(n);
        minimum.reserve(n);

        while (result < limit && bfs(s, t, level)) {
            fill(iter.begin(), iter.end(), 0);
            while (result < limit) {
                T pushed = augment(s, t, limit - result, level, iter, vertices, edge_indices, minimum);
                if (pushed == T(0)) break;
                result += pushed;
            }
        }
        return result;
    }

    // 辺取得: get_edge(i) -> 辺ID i の始点・終点・容量・流量 (前提: 0 <= i < 辺数)
    Edge get_edge(int i) const {
        assert(0 <= i && i < (int)pos.size());
        auto [from, index] = pos[i];
        const ResidualEdge& e = graph[from][index];
        const ResidualEdge& rev = graph[e.to][e.rev];
        return {from, e.to, e.cap + rev.cap, rev.cap};
    }

    // 全辺取得: edges() -> 辺ID順の辺情報の配列
    vector<Edge> edges() const {
        vector<Edge> result;
        result.reserve(pos.size());
        for (int i = 0; i < (int)pos.size(); i++) result.push_back(get_edge(i));
        return result;
    }

    // 最小カット側の頂点判定: min_cut(s) -> 残余グラフで s から到達可能な頂点 (前提: s が有効)
    vector<bool> min_cut(int s) const {
        assert(0 <= s && s < n);
        vector<bool> visited(n, false);
        queue<int> que;
        visited[s] = true;
        que.push(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (const ResidualEdge& e : graph[v]) {
                if (e.cap == T(0) || visited[e.to]) continue;
                visited[e.to] = true;
                que.push(e.to);
            }
        }
        return visited;
    }

    // 辺変更: change_edge(i, cap, flow) -> 辺ID i の容量・流量を変更 (前提: 0 <= flow <= cap、実行可能流の維持は呼び出し側の責任)
    void change_edge(int i, T new_cap, T new_flow) {
        assert(0 <= i && i < (int)pos.size());
        assert(T(0) <= new_flow && new_flow <= new_cap);
        auto [from, index] = pos[i];
        ResidualEdge& e = graph[from][index];
        ResidualEdge& rev = graph[e.to][e.rev];
        e.cap = new_cap - new_flow;
        rev.cap = new_flow;
    }
};
