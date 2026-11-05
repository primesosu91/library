#include <bits/stdc++.h>
using namespace std;

// 配列のZ配列: z[i] は v の先頭と位置 i からの最長一致長 (0-index)
// 引数: v は元の配列 (前提: v.size() <= INT_MAX、== は等値関係を表す)
// 戻り値: 空入力なら空、それ以外は z[0] = v.size()
// 等値比較 O(1) のとき時間 O(n)、返り値を含む追加領域 O(n)
template<class T>
vector<int> z_algorithm(const vector<T>& v) {
    assert(v.size() <= (size_t)INT_MAX);
    int n = (int)v.size();
    vector<int> z(n, 0);
    if (n == 0) return z;

    // [left, right) は先頭と一致する、右端が最も右にある確認済み区間
    int left = 0, right = 0;
    for (int i = 1; i < n; i++) {
        if (i < right) z[i] = min(right - i, z[i - left]);

        // 確認済み区間の外側だけを比較して一致を伸ばす
        while (z[i] < n - i && v[z[i]] == v[i + z[i]]) z[i]++;
        if (i + z[i] > right) {
            left = i;
            right = i + z[i];
        }
    }
    z[0] = n;
    return z;
}

// 文字列のZ配列: z[i] は s の先頭と位置 i からの最長一致長 (バイト単位)
// 引数: s は元の文字列 (前提: s.size() <= INT_MAX)
// 戻り値: 空入力なら空、それ以外は z[0] = s.size()
// 時間 O(n)、返り値・変換用配列を含む追加領域 O(n)
vector<int> z_algorithm(const string& s) {
    assert(s.size() <= (size_t)INT_MAX);
    return z_algorithm(vector<char>(s.begin(), s.end()));
}
