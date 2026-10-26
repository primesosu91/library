#include <bits/stdc++.h>
using namespace std;

struct TopologicalSort {
private:
    int n;
    int edge_count;
    bool built;
    bool dag;
    vector<vector<int>> graph;
    vector<int> sorted_order;

public:
    // 初期化: TopologicalSort(n) -> n 頂点の空の有向グラフ (前提: n >= 0)
    TopologicalSort(int vertex_count)
        : n(vertex_count), edge_count(0), built(false), dag(false) {
        assert(vertex_count >= 0);
        graph.resize(n);
    }

    // 有向辺追加: add_edge(from, to) -> 新しい辺ID (前提: 頂点が有効)
    int add_edge(int from, int to) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int id = edge_count++;
        graph[from].push_back(to);
        built = false;
        return id;
    }

    // トポロジカルソート: build(lexicographical) -> DAGならtrue (trueなら辞書順最小、閉路があれば順序は空)
    bool build(bool lexicographical = false) {
        built = false;
        sorted_order.clear();
        sorted_order.reserve(n);
        vector<int> indegree(n, 0);
        for (int v = 0; v < n; v++) {
            for (int to : graph[v]) indegree[to]++;
        }

        // 入次数0の頂点から順に取り出す（必要な場合のみ辞書順最小化）
        if (lexicographical) {
            priority_queue<int, vector<int>, greater<int>> que;
            for (int v = 0; v < n; v++) {
                if (indegree[v] == 0) que.push(v);
            }
            while (!que.empty()) {
                int v = que.top();
                que.pop();
                sorted_order.push_back(v);
                for (int to : graph[v]) {
                    if (--indegree[to] == 0) que.push(to);
                }
            }
        } else {
            queue<int> que;
            for (int v = 0; v < n; v++) {
                if (indegree[v] == 0) que.push(v);
            }
            while (!que.empty()) {
                int v = que.front();
                que.pop();
                sorted_order.push_back(v);
                for (int to : graph[v]) {
                    if (--indegree[to] == 0) que.push(to);
                }
            }
        }

        // 全頂点を取り出せなければ閉路があるため、部分的な順序は破棄する
        dag = ((int)sorted_order.size() == n);
        if (!dag) sorted_order.clear();
        built = true;
        return dag;
    }

    // DAG判定: is_dag() -> 閉路が存在しなければtrue (前提: build済み)
    bool is_dag() const {
        assert(built);
        return dag;
    }

    // 頂点順序: order() -> トポロジカル順序への参照 (閉路があれば空、前提: build済み、辺追加・再計算後は再取得)
    const vector<int>& order() const {
        assert(built);
        return sorted_order;
    }
};
