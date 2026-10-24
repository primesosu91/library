#include <bits/stdc++.h>
using namespace std;

struct FunctionalGraph {
private:
    int n;
    vector<int> ids;
    vector<int> positions;
    vector<int> distances;
    vector<int> entries;
    vector<vector<int>> cycles;
    vector<vector<int>> components;
    vector<int> tin;
    vector<int> tout;

    // 入次数0の頂点を取り除き、残った閉路と流入頂点の情報を構築する
    template<class T>
    void build(const vector<T>& next) {
        static_assert(is_integral_v<T> && is_signed_v<T> && sizeof(T) <= sizeof(long long),
                      "next must contain signed integers of at most 64 bits");
        assert(next.size() <= (size_t)INT_MAX);
        n = (int)next.size();
        ids.assign(n, -1);
        positions.assign(n, -1);
        distances.assign(n, 0);
        entries.assign(n, -1);
        cycles.clear();
        components.clear();

        vector<int> indegree(n, 0);
        for (int v = 0; v < n; v++) {
            assert(0 <= next[v] && next[v] < n);
            indegree[(int)next[v]]++;
        }

        queue<int> que;
        for (int v = 0; v < n; v++) {
            if (indegree[v] == 0) que.push(v);
        }
        vector<int> order;
        order.reserve(n);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            order.push_back(v);
            int to = (int)next[v];
            if (--indegree[to] == 0) que.push(to);
        }

        // 頂点を昇順に調べるため、サイクルIDは最小頂点番号の昇順になる
        for (int v = 0; v < n; v++) {
            if (indegree[v] == 0 || ids[v] != -1) continue;
            int id = (int)cycles.size();
            vector<int> vertices;
            int cur = v;
            do {
                ids[cur] = id;
                positions[cur] = (int)vertices.size();
                entries[cur] = cur;
                vertices.push_back(cur);
                cur = (int)next[cur];
            } while (cur != v);
            cycles.push_back(move(vertices));
        }

        // 削除順を逆にたどり、到達サイクルと入口と距離を伝播する
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int v = order[i];
            int to = (int)next[v];
            ids[v] = ids[to];
            entries[v] = entries[to];
            distances[v] = distances[to] + 1;
        }

        // 流入木の部分木を連続区間に割り当て、祖先関係を O(1) で判定できるようにする
        vector<int> subtree(n, 1);
        for (int v : order) subtree[(int)next[v]] += subtree[v];

        tin.assign(n, -1);
        tout.assign(n, -1);
        vector<int> cursor(n, -1);
        int timer = 0;
        for (const auto& cycle : cycles) {
            for (int root : cycle) {
                tin[root] = timer;
                timer += subtree[root];
                cursor[root] = tin[root] + 1;
            }
        }
        // 削除順を逆にたどれば、親が必ず子より先に処理される
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int v = order[i];
            int parent = (int)next[v];
            tin[v] = cursor[parent];
            cursor[parent] += subtree[v];
            cursor[v] = tin[v] + 1;
        }
        for (int v = 0; v < n; v++) tout[v] = tin[v] + subtree[v];

        // 成分の頂点列は頂点番号の昇順で保存する
        components.resize(cycles.size());
        for (int v = 0; v < n; v++) components[ids[v]].push_back(v);
    }

public:
    // 初期化: FunctionalGraph(next) -> 出次数1の有向グラフを解析する
    // 前提: next は符号付き整数配列、0 <= next[v] < N
    template<class T>
    explicit FunctionalGraph(const vector<T>& next) : n(0) {
        build(next);
    }

    // 頂点数: size() -> N
    int size() const { return n; }

    // サイクル数: cycle_count() -> 連結成分の個数
    int cycle_count() const { return (int)cycles.size(); }

    // 到達サイクル: cycle_id(v) -> 頂点vが最終的に到達するサイクルID
    int cycle_id(int v) const {
        assert(0 <= v && v < n);
        return ids[v];
    }

    // 到達サイクル長: cycle_length(v) -> 頂点vから到達するサイクルの長さ
    int cycle_length(int v) const {
        assert(0 <= v && v < n);
        return (int)cycles[ids[v]].size();
    }

    // サイクル長: cycle_size(id) -> サイクルidの長さ
    int cycle_size(int id) const {
        assert(0 <= id && id < cycle_count());
        return (int)cycles[id].size();
    }

    // サイクル頂点列: cycle_vertices(id) -> 最小頂点から辺の向きに沿った頂点列への参照
    const vector<int>& cycle_vertices(int id) const {
        assert(0 <= id && id < cycle_count());
        return cycles[id];
    }

    // サイクル内の位置: cycle_pos(v) -> サイクル外では -1
    int cycle_pos(int v) const {
        assert(0 <= v && v < n);
        return positions[v];
    }

    // サイクル上か: in_cycle(v) -> サイクルに属する場合true
    bool in_cycle(int v) const {
        assert(0 <= v && v < n);
        return distances[v] == 0;
    }

    // サイクルまでの距離: dist_to_cycle(v) -> 到達までの遷移回数
    int dist_to_cycle(int v) const {
        assert(0 <= v && v < n);
        return distances[v];
    }

    // 最初に到達するサイクル頂点: cycle_entry(v) -> 頂点番号
    int cycle_entry(int v) const {
        assert(0 <= v && v < n);
        return entries[v];
    }

    // 成分の大きさ: component_size(id) -> サイクルidに流入する頂点を含む個数
    int component_size(int id) const {
        assert(0 <= id && id < cycle_count());
        return (int)components[id].size();
    }

    // 成分の頂点列: component_vertices(id) -> 頂点番号昇順の頂点列への参照
    const vector<int>& component_vertices(int id) const {
        assert(0 <= id && id < cycle_count());
        return components[id];
    }

    // 到達判定: reachable(from, to) -> from から to に到達できるか
    bool reachable(int from, int to) const {
        return dist(from, to) != -1;
    }

    // 頂点間距離: dist(from, to) -> 最小遷移回数、到達不能なら -1
    int dist(int from, int to) const {
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);
        if (ids[from] != ids[to]) return -1;

        // to が流入木にある場合、to は from の祖先でなければならない
        if (distances[to] > 0) {
            if (tin[to] <= tin[from] && tin[from] < tout[to]) {
                return distances[from] - distances[to];
            }
            return -1;
        }

        // to がサイクル上なら、入口までの距離とサイクル上の距離を足す
        int length = (int)cycles[ids[from]].size();
        int from_pos = positions[entries[from]];
        int to_pos = positions[to];
        int cycle_dist = to_pos >= from_pos ? to_pos - from_pos
                                           : length - from_pos + to_pos;
        return distances[from] + cycle_dist;
    }

    // 到達可能頂点数: reachable_count(v) -> v から到達する異なる頂点の個数（v 自身を含む）
    int reachable_count(int v) const {
        assert(0 <= v && v < n);
        return distances[v] + (int)cycles[ids[v]].size();
    }
};
