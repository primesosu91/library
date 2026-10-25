#include <bits/stdc++.h>
using namespace std;

// 外積の符号: cross_sign(a, b) -> a x b の符号 (-1, 0, 1)
// 前提: T は 64-bit 以下の符号付き整数型
// 正なら a から b は反時計回り、負なら時計回り、0 なら平行
// ゼロベクトルを含む場合も 0 を返す
// (座標値は __int128 で演算できる範囲とする)
template<class T>
int cross_sign(const pair<T, T>& a, const pair<T, T>& b) {
    static_assert(is_integral_v<T> && is_signed_v<T> && sizeof(T) <= 8,
                  "T must be a signed integer type of at most 64 bits");
    __int128_t cross = (__int128_t)a.first * b.second - (__int128_t)a.second * b.first;
    return (cross > 0) - (cross < 0);
}

template<class T = long long>
struct ArgumentLess {
    static_assert(is_integral_v<T> && is_signed_v<T> && sizeof(T) <= 8,
                  "T must be a signed integer type of at most 64 bits");

    pair<T, T> origin;

    // 基準点の設定: ArgumentLess(origin) -> 原点を省略すると (0, 0)
    // 前提: 各点と基準点の座標差の絶対値は 10^18 以下
    ArgumentLess(pair<T, T> origin = {0, 0}) : origin(origin) {}

    // 偏角順比較: operator()(a, b) -> a が b より先なら true
    // (0, 0) 方向のゼロベクトルは先頭、角度は正の x 軸から反時計回り
    // 同方向は基準点からの距離が小さい順、同一点は同値
    // 前提: 各点と基準点の座標差の絶対値は 10^18 以下
    bool operator()(const pair<T, T>& a, const pair<T, T>& b) const {
        using i128 = __int128_t;
        i128 ax = (i128)a.first - origin.first;
        i128 ay = (i128)a.second - origin.second;
        i128 bx = (i128)b.first - origin.first;
        i128 by = (i128)b.second - origin.second;

        // 基準点と一致する点は、他のどの点よりも先に置く
        bool az = (ax == 0 && ay == 0);
        bool bz = (bx == 0 && by == 0);
        if (az != bz) return az;
        if (az) return false;

        // 正の x 軸から上半平面を先にし、負の x 軸から下半平面を後にする
        auto half = [](i128 x, i128 y) {
            return (y > 0 || (y == 0 && x > 0)) ? 0 : 1;
        };
        int ha = half(ax, ay), hb = half(bx, by);
        if (ha != hb) return ha < hb;

        i128 cross = ax * by - ay * bx;
        if (cross != 0) return cross > 0;

        // 同方向のベクトルは距離の二乗で比較
        i128 da = ax * ax + ay * ay;
        i128 db = bx * bx + by * by;
        return da < db;
    }
};

// 偏角ソート: argument_sort(points, origin) -> 点列をその場で昇順ソート
// 前提: 各点と基準点の座標差の絶対値は 10^18 以下
// 基準点 origin を省略すると (0, 0)、ゼロベクトルは先頭
template<class T>
void argument_sort(vector<pair<T, T>>& points, pair<T, T> origin = {0, 0}) {
    sort(points.begin(), points.end(), ArgumentLess<T>(origin));
}
