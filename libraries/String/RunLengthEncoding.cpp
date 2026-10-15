#include <bits/stdc++.h>
using namespace std;

// 配列をランレングス圧縮: 連続する等しい要素を {値, 個数} に変換
// 引数: v は元の配列 (前提: v.size() <= INT_MAX、T は等値比較・コピー可能)
// 戻り値: 圧縮結果を元の順序で並べた vector<pair<T, int>>
template<class T>
vector<pair<T, int>> run_length_encode(const vector<T>& v) {
    assert(v.size() <= (size_t)INT_MAX);

    vector<pair<T, int>> result;
    for (const T& x : v) {
        if (result.empty() || !(result.back().first == x)) {
            result.emplace_back(x, 1);
        } else {
            result.back().second++;
        }
    }
    return result;
}

// 文字列をランレングス圧縮: 連続する同じバイトを {文字, 個数} に変換
// 引数: s は元の文字列 (前提: s.size() <= INT_MAX)
// 戻り値: vector<pair<char, int>>。文字はバイト単位で扱う
vector<pair<char, int>> run_length_encode(const string& s) {
    assert(s.size() <= (size_t)INT_MAX);

    vector<pair<char, int>> result;
    for (char c : s) {
        if (result.empty() || result.back().first != c) {
            result.emplace_back(c, 1);
        } else {
            result.back().second++;
        }
    }
    return result;
}

// 配列の復元: 各 {値, 個数} を個数分だけ並べる
// 引数: rle は圧縮結果 (前提: すべての個数が正、合計要素数が格納可能)
// 戻り値: 復元された vector<T>
template<class T>
vector<T> run_length_decode(const vector<pair<T, int>>& rle) {
    vector<T> result;
    size_t total = 0;
    for (const auto& [x, count] : rle) {
        assert(count > 0);
        assert((size_t)count <= result.max_size() - total);
        total += (size_t)count;
    }

    result.reserve(total);
    for (const auto& [x, count] : rle) {
        result.insert(result.end(), count, x);
    }
    return result;
}

// 文字列の復元: 各 {文字, 個数} を個数分だけ連結
// 引数: rle は圧縮結果 (前提: すべての個数が正、合計文字数が格納可能)
// 戻り値: 復元された string
string run_length_decode_string(const vector<pair<char, int>>& rle) {
    string result;
    size_t total = 0;
    for (const auto& [c, count] : rle) {
        assert(count > 0);
        assert((size_t)count <= result.max_size() - total);
        total += (size_t)count;
    }

    result.reserve(total);
    for (const auto& [c, count] : rle) {
        result.append(count, c);
    }
    return result;
}
