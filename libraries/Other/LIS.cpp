#include <bits/stdc++.h>
using namespace std;

template<class T>
struct LIS {
private:
    vector<T> data;
    vector<int> dp;
    vector<int> prev;
    int last;
    int lis_length;

public:
    // 初期化: LIS(配列 v, 狭義増加 strict = true) -> LIS と復元情報を O(n log n) で構築
    // 前提: T は大小比較・コピーが可能。strict = false なら広義増加を扱う
    LIS(const vector<T>& v, bool strict = true)
        : data(v), dp(v.size()), prev(v.size(), -1), last(-1), lis_length(0) {
        assert(v.size() <= (size_t)INT_MAX);

        vector<T> tail;
        vector<int> tail_index;
        tail.reserve(v.size());
        tail_index.reserve(v.size());

        for (int i = 0; i < (int)v.size(); i++) {
            // 長さ pos + 1 の増加部分列を、末尾の値が最小となるよう更新する
            int pos;
            if (strict) {
                pos = (int)(lower_bound(tail.begin(), tail.end(), v[i]) - tail.begin());
            } else {
                pos = (int)(upper_bound(tail.begin(), tail.end(), v[i]) - tail.begin());
            }

            dp[i] = pos + 1;
            if (pos > 0) prev[i] = tail_index[pos - 1];
            if (pos == (int)tail.size()) {
                tail.push_back(v[i]);
                tail_index.push_back(i);
            } else {
                tail[pos] = v[i];
                tail_index[pos] = i;
            }
        }

        lis_length = (int)tail.size();
        if (lis_length > 0) last = tail_index.back();
    }

    // 最大長: length() -> LIS 全体の最大長
    int length() const {
        return lis_length;
    }

    // 添字列の復元: indices() -> LIS を構成する元配列の 0-index 添字列（最適解のうち1つ）
    vector<int> indices() const {
        vector<int> result;
        for (int i = last; i != -1; i = prev[i]) result.push_back(i);
        reverse(result.begin(), result.end());
        return result;
    }

    // 値列の復元: sequence() -> LIS を構成する値列（最適解のうち1つ）
    vector<T> sequence() const {
        vector<T> result;
        for (int i = last; i != -1; i = prev[i]) result.push_back(data[i]);
        reverse(result.begin(), result.end());
        return result;
    }

    // 各位置の最大長: lengths() -> dp[i] は必ず位置 i で終わる LIS の長さ（参照を返す）
    const vector<int>& lengths() const {
        return dp;
    }
};
