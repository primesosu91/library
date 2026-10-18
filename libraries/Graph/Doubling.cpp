#include <bits/stdc++.h>
using namespace std;

struct Doubling {
private:
    int n;
    long long limit;
    bool has_sum;
    vector<vector<int>> table;
    vector<vector<long long>> sum_table;

    // 遷移先を 2^j 回分まとめて前計算する
    template<class T>
    void build_next(const vector<T>& next) {
        static_assert(is_integral_v<T> && is_signed_v<T> && sizeof(T) <= sizeof(long long), "next must contain signed integers of at most 64 bits");
        assert(next.size() <= (size_t)INT_MAX && limit >= 0);
        n = (int)next.size();
        int log = 1;
        while ((limit >> log) != 0) log++;
        table.assign(log, vector<int>(n, -1));

        for (int v = 0; v < n; v++) {
            assert(next[v] == -1 || (0 <= next[v] && next[v] < n));
            table[0][v] = (int)next[v];
        }
        for (int j = 1; j < log; j++) {
            for (int v = 0; v < n; v++) {
                int to = table[j - 1][v];
                if (to != -1) table[j][v] = table[j - 1][to];
            }
        }
    }

    // 重み付きの場合だけ集約テーブルを構築する
    template<class T, class U>
    void build_weighted(const vector<T>& next, const vector<U>& value) {
        static_assert(is_integral_v<U> && is_signed_v<U> && sizeof(U) <= sizeof(long long), "value must contain signed integers of at most 64 bits");
        assert(next.size() == value.size());
        build_next(next);
        sum_table.assign(table.size(), vector<long long>(n, 0));
        for (int v = 0; v < n; v++) sum_table[0][v] = (long long)value[v];

        // sum_table[j][v]: v から最大 2^j 個の頂点の重みの和 (遷移終了で打ち切り)
        for (int j = 1; j < (int)table.size(); j++) {
            for (int v = 0; v < n; v++) {
                int to = table[j - 1][v];
                __int128 total = (__int128)sum_table[j - 1][v];
                if (to != -1) total += sum_table[j - 1][to];
                assert(LLONG_MIN <= total && total <= LLONG_MAX);
                sum_table[j][v] = (long long)total;
            }
        }
    }

public:
    // 初期化: Doubling(next, max_step) -> 遷移先のみを構築 (-1 は遷移終了)
    // 前提: next は符号付き整数配列、next[v] は -1 または [0, N)、max_step >= 0
    Doubling(const vector<int>& next, long long max_step = (1LL << 60) - 1)
        : n(0), limit(max_step), has_sum(false) {
        build_next(next);
    }

    // 初期化: Doubling(next, max_step) -> long long などの符号付き整数配列に対応
    // 前提: next[v] は -1 または [0, N)、max_step >= 0
    template<class T>
    Doubling(const vector<T>& next, long long max_step = (1LL << 60) - 1)
        : n(0), limit(max_step), has_sum(false) {
        build_next(next);
    }

    // 初期化リストでも使える重み付き初期化 (next は int、value は long long)
    // 前提: value は next と同じ長さ、各集約和は long long に収まる
    Doubling(const vector<int>& next, const vector<long long>& value, long long max_step)
        : n(0), limit(max_step), has_sum(true) {
        build_weighted(next, value);
    }

    // 重み付き初期化: next と value は int / long long などの符号付き整数配列
    // 前提: next と value は同じ長さ、各集約和は long long に収まる
    template<class T, class U>
    Doubling(const vector<T>& next, const vector<U>& value, long long max_step)
        : n(0), limit(max_step), has_sum(true) {
        build_weighted(next, value);
    }

    // k 回遷移: jump(v, k) -> k 回後の頂点 (途中で遷移が終了したら -1)
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

    // 重みの和: sum(v, k) -> v から始めて k 個の頂点の重みの和 (遷移終了で打ち切り)
    // 前提: 重み付き初期化済み、0 <= v < N、0 <= k <= max_step、和は long long に収まる
    long long sum(int v, long long k) const {
        assert(has_sum && 0 <= v && v < n);
        assert(0 <= k && k <= limit);
        long long result = 0;

        for (int j = 0; k > 0; j++, k >>= 1) {
            if (k & 1) {
                __int128 total = (__int128)result + sum_table[j][v];
                assert(LLONG_MIN <= total && total <= LLONG_MAX);
                result = (long long)total;
                v = table[j][v];
                if (v == -1) break;
            }
        }
        return result;
    }
};
