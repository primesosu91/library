#include <bits/stdc++.h>
using namespace std;

template<class T = long long>
struct ProjectSelection {
    static_assert(is_integral_v<T> && is_signed_v<T> && sizeof(T) <= 8,
                  "T must be a signed integer type of at most 64 bits");

public:
    struct Result {
        T value;
        vector<bool> selected;
        bool feasible;
    };

private:
    using Wide = __int128_t;
    static constexpr int SOURCE = -1;
    static constexpr int SINK = -2;

    struct Request {
        int from;
        int to;
        Wide cap;
        bool hard;
    };

    struct Dinic {
        struct Edge {
            int to;
            int rev;
            Wide cap;
        };

        vector<vector<Edge>> graph;

        Dinic(int n) : graph(n) {}

        void add_edge(int from, int to, Wide cap) {
            assert(cap >= 0);
            int fi = (int)graph[from].size();
            int ti = (int)graph[to].size();
            if (from == to) ti++;
            graph[from].push_back({to, ti, cap});
            graph[to].push_back({from, fi, 0});
        }

        bool bfs(int s, int t, vector<int>& level) const {
            fill(level.begin(), level.end(), -1);
            queue<int> que;
            level[s] = 0;
            que.push(s);
            while (!que.empty()) {
                int v = que.front();
                que.pop();
                for (const Edge& e : graph[v]) {
                    if (e.cap == 0 || level[e.to] != -1) continue;
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
            return level[t] != -1;
        }

        // 再帰せずにレベルグラフ上の増加路を1本見つけて流す
        Wide augment(int s, int t, Wide limit, const vector<int>& level, vector<int>& iter,
                     vector<int>& vertices, vector<int>& edge_indices, vector<Wide>& minimum) {
            vertices.clear();
            edge_indices.clear();
            minimum.clear();
            vertices.push_back(s);
            minimum.push_back(limit);

            while (!vertices.empty()) {
                int v = vertices.back();
                if (v == t) {
                    Wide pushed = minimum.back();
                    for (int i = 0; i < (int)edge_indices.size(); i++) {
                        Edge& e = graph[vertices[i]][edge_indices[i]];
                        e.cap -= pushed;
                        graph[e.to][e.rev].cap += pushed;
                    }
                    return pushed;
                }

                int& i = iter[v];
                while (i < (int)graph[v].size()) {
                    const Edge& e = graph[v][i];
                    if (e.cap > 0 && level[e.to] == level[v] + 1) break;
                    i++;
                }
                if (i == (int)graph[v].size()) {
                    vertices.pop_back();
                    minimum.pop_back();
                    if (vertices.empty()) break;
                    edge_indices.pop_back();
                    iter[vertices.back()]++;
                } else {
                    const Edge& e = graph[v][i];
                    edge_indices.push_back(i);
                    vertices.push_back(e.to);
                    minimum.push_back(min(minimum.back(), e.cap));
                }
            }
            return 0;
        }

        Wide flow(int s, int t, Wide limit) {
            Wide result = 0;
            int n = (int)graph.size();
            vector<int> level(n), iter(n), vertices, edge_indices;
            vector<Wide> minimum;
            vertices.reserve(n);
            edge_indices.reserve(n);
            minimum.reserve(n);

            while (result < limit && bfs(s, t, level)) {
                fill(iter.begin(), iter.end(), 0);
                while (result < limit) {
                    Wide pushed = augment(s, t, limit - result, level, iter,
                                          vertices, edge_indices, minimum);
                    if (pushed == 0) break;
                    result += pushed;
                }
            }
            return result;
        }

        vector<bool> min_cut(int s) const {
            vector<bool> visited(graph.size(), false);
            queue<int> que;
            visited[s] = true;
            que.push(s);
            while (!que.empty()) {
                int v = que.front();
                que.pop();
                for (const Edge& e : graph[v]) {
                    if (e.cap == 0 || visited[e.to]) continue;
                    visited[e.to] = true;
                    que.push(e.to);
                }
            }
            return visited;
        }
    };

    int n;
    int vertex_count;
    Wide base_value = 0;
    Wide finite_sum = 0;
    vector<Request> requests;

    bool valid(int i) const {
        return 0 <= i && i < n;
    }

    void add_edge(int from, int to, Wide cap) {
        assert(cap >= 0);
        if (cap == 0) return;
        requests.push_back({from, to, cap, false});
        const Wide wide_max = (Wide)(((__uint128_t)1 << 127) - 1);
        assert(finite_sum <= wide_max - cap - 1);
        finite_sum += cap;
    }

    void add_hard_edge(int from, int to) {
        requests.push_back({from, to, 0, true});
    }

    // 頂点が false / true のときの利益を最小カット用に変換する
    void add_unary(int i, Wide v0, Wide v1) {
        base_value += max(v0, v1);
        if (v0 < v1) add_edge(SOURCE, i, v1 - v0);
        if (v0 > v1) add_edge(i, SINK, v0 - v1);
    }

public:
    // 初期化: ProjectSelection(n) -> n 個の二択変数 (前提: n >= 0)
    ProjectSelection(int variable_count) : n(variable_count), vertex_count(variable_count) {
        assert(variable_count >= 0);
    }

    // 定数利益: add_value(value) -> 全ての割当てに value を加算
    void add_value(T value) {
        base_value += Wide(value);
    }

    // 単項利益: add_value(i, v0, v1) -> i=false/true 時の利益 (nullopt はその状態を禁止)
    void add_value(int i, optional<T> v0, optional<T> v1) {
        assert(valid(i));
        add_unary(i, Wide(v0.value_or(0)), Wide(v1.value_or(0)));
        if (!v0) add_hard_edge(SOURCE, i);
        if (!v1) add_hard_edge(i, SINK);
    }

    // 二項利益: add_pairwise(i, j, v00, v01, v10, v11) -> (i, j) の各状態に対応する利益 (nullopt は禁止状態)
    // 前提: i != j、4状態すべてを許容する場合は v00 + v11 >= v01 + v10
    void add_pairwise(int i, int j, optional<T> v00, optional<T> v01,
                      optional<T> v10, optional<T> v11) {
        assert(valid(i) && valid(j) && i != j);
        bool f00 = !v00, f01 = !v01, f10 = !v10, f11 = !v11;
        // 00 または 11 を禁止する場合は、01 または 10 の少なくとも一方も禁止する
        assert((!f00 || f01 || f10) && (!f11 || f01 || f10));

        Wide a = Wide(v00.value_or(0));
        Wide b = Wide(v01.value_or(0));
        Wide c = Wide(v10.value_or(0));
        Wide d = Wide(v11.value_or(0));
        Wide delta = a + d - b - c;
        if (delta < 0) {
            // 禁止状態の値は自由に補完できる。利益表が優モジュラになるよう調整する
            assert(f01 || f10);
            if (f01) b += delta;
            else c += delta;
            delta = 0;
        }

        // a + (d-b)*i + (b-a)*j - delta*i*(1-j)
        base_value += a;
        add_unary(i, 0, d - b);
        add_unary(j, 0, b - a);
        add_edge(i, j, delta);

        // 禁止状態を、含意または単項の強制条件に分解する
        if (f10) add_hard_edge(i, j);
        if (f01) add_hard_edge(j, i);
        if (f00 && f01) add_hard_edge(SOURCE, i);
        if (f00 && f10) add_hard_edge(SOURCE, j);
        if (f11 && f01) add_hard_edge(j, SINK);
        if (f11 && f10) add_hard_edge(i, SINK);
    }

    // 全一致ボーナス: add_all(ids, selected, value) -> 全頂点が selected なら value を加算
    // 前提: ids は相異なる有効な頂点番号を1個以上含み、value >= 0
    void add_all(const vector<int>& ids, bool selected, T value) {
        assert(!ids.empty() && value >= T(0));
        for (int i : ids) assert(valid(i));
        vector<int> sorted = ids;
        sort(sorted.begin(), sorted.end());
        assert(adjacent_find(sorted.begin(), sorted.end()) == sorted.end());
        if (value == T(0)) return;
        if (ids.size() == 1) {
            if (selected) add_unary(ids[0], 0, Wide(value));
            else add_unary(ids[0], Wide(value), 0);
            return;
        }
        assert(vertex_count < numeric_limits<int>::max() - 2);
        int aux = vertex_count++;
        if (selected) {
            // 補助変数が true なら、指定された全変数も true
            add_unary(aux, 0, Wide(value));
            for (int i : ids) add_hard_edge(aux, i);
        } else {
            // 補助変数が false なら、指定された全変数も false
            add_unary(aux, Wide(value), 0);
            for (int i : ids) add_hard_edge(i, aux);
        }
    }

    // 最大利益: solve() -> {最大利益, 各変数の選択状態, 実行可能性}
    // 解なしの場合は {0, {}, false}。何度呼んでも元の条件から再計算し、条件追加後にも使用可能
    Result solve() const {
        int s = vertex_count, t = vertex_count + 1;
        Wide inf = finite_sum + 1;
        Dinic mf(vertex_count + 2);
        for (const Request& e : requests) {
            int from = (e.from == SOURCE ? s : e.from == SINK ? t : e.from);
            int to = (e.to == SOURCE ? s : e.to == SINK ? t : e.to);
            mf.add_edge(from, to, e.hard ? inf : e.cap);
        }
        Wide cost = mf.flow(s, t, inf);
        if (cost == inf) return Result{T(0), {}, false};

        Wide answer = base_value - cost;
        assert(Wide(numeric_limits<T>::min()) <= answer &&
               answer <= Wide(numeric_limits<T>::max()));
        vector<bool> cut = mf.min_cut(s);
        vector<bool> selected(n);
        for (int i = 0; i < n; i++) selected[i] = cut[i];
        return Result{T(answer), move(selected), true};
    }
};
