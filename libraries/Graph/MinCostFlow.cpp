#include <bits/stdc++.h>
using namespace std;

template<class Cap = long long, class Cost = long long>
struct MinCostFlow {
    static_assert(is_integral_v<Cap> && !is_same_v<Cap, bool>, "Cap must be an integer type");
    static_assert(is_integral_v<Cost> && is_signed_v<Cost>, "Cost must be a signed integer type");

public:
    struct Edge {
        int from;
        int to;
        Cap cap;
        Cap flow;
        Cost cost;
    };

private:
    using Wide = __int128_t;

    struct ResidualEdge {
        int to;
        int rev;
        Cap cap;
        Cost cost;
    };

    int n;
    int source, sink;
    vector<vector<ResidualEdge>> graph;
    vector<pair<int, int>> pos;
    vector<Wide> potential;

    // 現在の残余グラフから追加流量を求める（slope が必要な場合は履歴も記録）
    pair<Cap, Cost> run(int s, int t, Cap limit, vector<pair<Cap, Cost>>* history) {
        assert(0 <= s && s < n && 0 <= t && t < n && s != t);
        assert(limit >= Cap(0));
        if (source == -1) {
            source = s;
            sink = t;
        } else {
            assert(s == source && t == sink);
        }

        if (history) history->push_back({Cap(0), Cost(0)});
        Cap total_flow = Cap(0);
        Cost total_cost = Cost(0);
        Wide last_unit_cost = 0;
        bool has_last = false;
        const Wide inf = Wide(1) << 120;
        vector<Wide> dist(n);
        vector<int> prev_vertex(n), prev_edge(n);

        while (total_flow < limit) {
            fill(dist.begin(), dist.end(), inf);
            fill(prev_vertex.begin(), prev_vertex.end(), -1);
            priority_queue<pair<Wide, int>, vector<pair<Wide, int>>,
                           greater<pair<Wide, int>>> pq;
            dist[s] = 0;
            pq.push({0, s});

            // ポテンシャルで非負に再重み付けし、最短増加路を探索する
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();
                if (d != dist[v]) continue;
                for (int i = 0; i < (int)graph[v].size(); i++) {
                    const ResidualEdge& e = graph[v][i];
                    if (e.cap == Cap(0)) continue;
                    Wide reduced_cost = Wide(e.cost) + potential[v] - potential[e.to];
                    assert(reduced_cost >= 0);
                    Wide next_dist = d + reduced_cost;
                    if (next_dist >= dist[e.to]) continue;
                    dist[e.to] = next_dist;
                    prev_vertex[e.to] = v;
                    prev_edge[e.to] = i;
                    pq.push({next_dist, e.to});
                }
            }
            if (dist[t] == inf) break;

            // 更新後のポテンシャル差が、今回の単位流量あたりの費用となる
            for (int v = 0; v < n; v++) {
                if (dist[v] != inf) potential[v] += dist[v];
            }
            Wide unit_cost = potential[t] - potential[s];
            assert(0 <= unit_cost && unit_cost <= Wide(numeric_limits<Cost>::max()));

            Cap pushed = limit - total_flow;
            for (int v = t; v != s; v = prev_vertex[v]) {
                const ResidualEdge& e = graph[prev_vertex[v]][prev_edge[v]];
                pushed = min(pushed, e.cap);
            }

            // 総費用が Cost に収まることを確認してから残余容量を更新する
            Wide next_cost = Wide(total_cost) + Wide(pushed) * unit_cost;
            assert(next_cost <= Wide(numeric_limits<Cost>::max()));
            for (int v = t; v != s; v = prev_vertex[v]) {
                ResidualEdge& e = graph[prev_vertex[v]][prev_edge[v]];
                e.cap -= pushed;
                graph[v][e.rev].cap += pushed;
            }
            total_flow += pushed;
            total_cost = Cost(next_cost);

            // 傾きが等しい連続区間は1本の線分としてまとめる
            if (history) {
                if (has_last && last_unit_cost == unit_cost) history->pop_back();
                history->push_back({total_flow, total_cost});
            }
            last_unit_cost = unit_cost;
            has_last = true;
        }
        return {total_flow, total_cost};
    }

public:
    // 初期化: MinCostFlow(n) -> n 頂点の空グラフ (前提: n >= 0)
    MinCostFlow(int vertex_count) : n(vertex_count), source(-1), sink(-1) {
        assert(vertex_count >= 0);
        graph.resize(n);
        potential.assign(n, 0);
    }

    // 有向辺追加: add_edge(from, to, cap, cost) -> 辺ID (前提: 頂点が有効、cap >= 0、cost >= 0、流量計算前)
    int add_edge(int from, int to, Cap cap, Cost cost) {
        assert(source == -1);
        assert(0 <= from && from < n && 0 <= to && to < n);
        assert(cap >= Cap(0) && cost >= Cost(0));
        int id = (int)pos.size();
        int from_id = (int)graph[from].size();
        int to_id = (int)graph[to].size();
        if (from == to) to_id++;
        pos.push_back({from, from_id});
        graph[from].push_back({to, to_id, cap, cost});
        graph[to].push_back({from, from_id, Cap(0), Cost(-cost)});
        return id;
    }

    // 追加最小費用最大流: flow(s, t) -> 現在の残余グラフから追加した {流量, 追加費用} (前提: s != t、以後も同じ s, t)
    pair<Cap, Cost> flow(int s, int t) {
        return flow(s, t, numeric_limits<Cap>::max());
    }

    // 流量制限付き: flow(s, t, limit) -> limit 以下で今回追加した {流量, 追加費用} (前提: limit >= 0、同じ s, t)
    pair<Cap, Cost> flow(int s, int t, Cap limit) {
        return run(s, t, limit, nullptr);
    }

    // 追加費用曲線: slope(s, t) -> 現在の残余グラフから追加可能な流量と追加費用の折れ線頂点 (前提: 同じ s, t)
    // 対応する流量を実際に追加し、返り値は今回の追加分のみを表す
    vector<pair<Cap, Cost>> slope(int s, int t) {
        return slope(s, t, numeric_limits<Cap>::max());
    }

    // 流量制限付き費用曲線: slope(s, t, limit) -> {0,0} から始まる今回の追加流量と追加費用の折れ線頂点
    // 前提: limit >= 0、同じ s, t。例: flow(s,t,2) の後の slope(s,t,3) は、さらに最大3流す分のみを返す
    vector<pair<Cap, Cost>> slope(int s, int t, Cap limit) {
        vector<pair<Cap, Cost>> result;
        run(s, t, limit, &result);
        return result;
    }

    // 辺取得: get_edge(i) -> 辺ID i の始点・終点・容量・流量・コスト (前提: 0 <= i < 辺数)
    Edge get_edge(int i) const {
        assert(0 <= i && i < (int)pos.size());
        auto [from, index] = pos[i];
        const ResidualEdge& e = graph[from][index];
        const ResidualEdge& rev = graph[e.to][e.rev];
        return {from, e.to, e.cap + rev.cap, rev.cap, e.cost};
    }

    // 全辺取得: edges() -> 辺ID順の辺情報の配列
    vector<Edge> edges() const {
        vector<Edge> result;
        result.reserve(pos.size());
        for (int i = 0; i < (int)pos.size(); i++) result.push_back(get_edge(i));
        return result;
    }
};
