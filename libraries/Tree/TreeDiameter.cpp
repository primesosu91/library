#include <bits/stdc++.h>
using namespace std;

// 非負整数重みの木の直径: build・メモリ O(n)、長さ・両端取得 O(1)、経路取得 O(経路長)
// 距離の合計は long long の範囲内。直径が複数ある場合はそのうち1つを返す。
struct TreeDiameter {
private:
    int n;
    int edge_count;
    bool built;
    vector<vector<pair<int, long long>>> graph;
    vector<int> parents;
    int start, goal;
    long long length;

    // 木の唯一の経路に沿って距離を計算し、最遠頂点と距離を返す。
    pair<int, long long> farthest(int root) {
        vector<long long> distances(n, -1);
        vector<int> order;
        order.reserve(n);
        parents.assign(n, -1);
        distances[root] = 0;
        order.push_back(root);
        int result = root;
        for (int i = 0; i < (int)order.size(); i++) {
            int v = order[i];
            for (auto [to, cost] : graph[v]) {
                if (distances[to] != -1) continue;
                assert(distances[v] <= numeric_limits<long long>::max() - cost);
                distances[to] = distances[v] + cost;
                parents[to] = v;
                order.push_back(to);
                if (distances[to] > distances[result]) result = to;
            }
        }
        assert((int)order.size() == n);
        return {result, distances[result]};
    }

public:
    // 初期化: TreeDiameter(n) -> n頂点の空グラフ (前提: n >= 1、頂点は0-index)
    TreeDiameter(int vertex_count) : n(vertex_count), edge_count(0), built(false),
                                    start(0), goal(0), length(0) {
        assert(n >= 1);
        graph.resize(n);
    }

    // 無向辺追加: add_edge(u,v,cost=1) -> 返り値なし (前提: 頂点が有効、cost >= 0)
    // 追加後は取得APIを使う前に再buildが必要。
    void add_edge(int u, int v, long long cost = 1) {
        assert(0 <= u && u < n && 0 <= v && v < n && cost >= 0);
        graph[u].emplace_back(v, cost);
        graph[v].emplace_back(u, cost);
        edge_count++;
        built = false;
    }

    // 構築: build() -> 直径を計算し直す、返り値なし (前提: グラフは木)
    void build() {
        assert(edge_count == n - 1);
        built = false;
        start = farthest(0).first;
        auto result = farthest(start);
        goal = result.first;
        length = result.second;
        built = true;
    }

    // 直径取得: diameter() -> 直径の重みの合計 (前提: build済み)
    long long diameter() const {
        assert(built);
        return length;
    }

    // 両端取得: endpoints() -> {始点, 終点} (前提: build済み)
    // 1頂点では{0,0}。全辺の重みが0の場合も同じ頂点を返すことがある。
    pair<int, int> endpoints() const {
        assert(built);
        return {start, goal};
    }

    // 経路取得: path() -> 始点から終点への頂点列のコピー (前提: build済み)
    // 両端を含む。始点と終点が同じ場合はその1頂点を返す。
    vector<int> path() const {
        assert(built);
        vector<int> result;
        for (int v = goal; v != -1; v = parents[v]) result.push_back(v);
        reverse(result.begin(), result.end());
        return result;
    }
};
