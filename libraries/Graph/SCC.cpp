#include <bits/stdc++.h>
using namespace std;

struct SCC {
private:
    int n;
    int edge_count;
    bool built;
    vector<vector<int>> graph;
    vector<vector<int>> reverse_graph;
    vector<int> ids;
    vector<vector<int>> components;
    vector<vector<int>> condensed_graph;

public:
    // 初期化: SCC(n) -> n 頂点の空の有向グラフ (前提: n >= 0)
    SCC(int vertex_count) : n(vertex_count), edge_count(0), built(false) {
        assert(vertex_count >= 0);
        graph.resize(n);
        reverse_graph.resize(n);
    }

    // 有向辺追加: add_edge(from, to) -> 新しい辺ID (前提: 頂点が有効)
    int add_edge(int from, int to) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int id = edge_count++;
        graph[from].push_back(to);
        reverse_graph[to].push_back(from);
        built = false;
        return id;
    }

    // 強連結成分分解: build() -> 成分IDと縮約DAGを構築 (成分IDはトポロジカル順)
    void build() {
        built = false;
        vector<char> visited(n, false);
        vector<int> next_index(n, 0), order, stack;
        order.reserve(n);
        stack.reserve(n);

        // 元のグラフで終了順を求める（非再帰DFS）
        for (int root = 0; root < n; root++) {
            if (visited[root]) continue;
            visited[root] = true;
            stack.push_back(root);
            while (!stack.empty()) {
                int v = stack.back();
                if (next_index[v] == (int)graph[v].size()) {
                    order.push_back(v);
                    stack.pop_back();
                    continue;
                }
                int to = graph[v][next_index[v]++];
                if (visited[to]) continue;
                visited[to] = true;
                stack.push_back(to);
            }
        }

        // 逆向きグラフを終了順の逆順に探索し、成分IDを割り当てる
        ids.assign(n, -1);
        int count = 0;
        for (int i = n - 1; i >= 0; i--) {
            int root = order[i];
            if (ids[root] != -1) continue;
            ids[root] = count;
            stack.push_back(root);
            while (!stack.empty()) {
                int v = stack.back();
                stack.pop_back();
                for (int to : reverse_graph[v]) {
                    if (ids[to] != -1) continue;
                    ids[to] = count;
                    stack.push_back(to);
                }
            }
            count++;
        }

        // 各成分の頂点を昇順で格納する
        components.assign(count, {});
        for (int v = 0; v < n; v++) components[ids[v]].push_back(v);

        // 成分間の辺を重複なく格納する（成分ごとに訪問先を記録）
        condensed_graph.assign(count, {});
        vector<int> last_seen(count, -1);
        for (int id = 0; id < count; id++) {
            for (int v : components[id]) {
                for (int to : graph[v]) {
                    int next_id = ids[to];
                    if (next_id == id || last_seen[next_id] == id) continue;
                    last_seen[next_id] = id;
                    condensed_graph[id].push_back(next_id);
                }
            }
        }
        built = true;
    }

    // 成分数: component_count() -> 強連結成分の個数 (前提: build 済み)
    int component_count() const {
        assert(built);
        return (int)components.size();
    }

    // 所属成分: component_id(v) -> 頂点vの成分ID (前提: build 済み、0 <= v < n)
    int component_id(int v) const {
        assert(built && 0 <= v && v < n);
        return ids[v];
    }

    // 同一成分判定: same(u, v) -> 互いに到達可能ならtrue (前提: build 済み、頂点が有効)
    bool same(int u, int v) const {
        assert(built && 0 <= u && u < n && 0 <= v && v < n);
        return ids[u] == ids[v];
    }

    // 成分一覧: groups() -> 成分ID順の頂点列への参照（各成分内は頂点昇順、前提: build 済み）
    const vector<vector<int>>& groups() const {
        assert(built);
        return components;
    }

    // 縮約DAG: dag() -> 成分間の重複のない有向辺の隣接リストへの参照（前提: build 済み）
    const vector<vector<int>>& dag() const {
        assert(built);
        return condensed_graph;
    }
};
