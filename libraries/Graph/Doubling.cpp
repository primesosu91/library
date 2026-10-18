#include <bits/stdc++.h>
using namespace std;

struct Doubling {
private:
    int n;
    long long limit;
    vector<vector<int>> table;

public:
    // 初期化: Doubling(next, max_step) -> next[v] を1回の遷移先として構築 (-1は遷移終了)
    // 前提: next[v] は -1 または [0, N) の頂点番号、max_step >= 0
    Doubling(const vector<int>& next, long long max_step = (1LL << 60) - 1)
        : n((int)next.size()), limit(max_step) {
        assert(max_step >= 0);
        int log = 1;
        while ((max_step >> log) != 0) log++;
        table.assign(log, vector<int>(n, -1));

        for (int v = 0; v < n; v++) {
            assert(next[v] == -1 || (0 <= next[v] && next[v] < n));
            table[0][v] = next[v];
        }

        // table[j][v]: 頂点 v から 2^j 回遷移した先
        for (int j = 1; j < log; j++) {
            for (int v = 0; v < n; v++) {
                int to = table[j - 1][v];
                if (to != -1) table[j][v] = table[j - 1][to];
            }
        }
    }

    // k回遷移: jump(v, k) -> k回後の頂点 (途中で遷移が終了したら -1)
    // 前提: 0 <= v < N、0 <= k <= max_step
    int jump(int v, long long k) const {
        assert(0 <= v && v < n);
        assert(0 <= k && k <= limit);

        for (int j = 0; k > 0; j++, k >>= 1) {
            if (k & 1) {
                v = table[j][v];
                if (v == -1) return -1;
            }
        }
        return v;
    }
};
