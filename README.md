# library

競技プログラミング向けの自作C++ライブラリです。GNU++17を基本とし、ACLや外部マクロには依存しません。
ライブラリ本体は `libraries/` 以下のカテゴリ別フォルダに配置します。

## DataStructure

- [SegmentTree](libraries/DataStructure/SegmentTree.cpp)：点更新、点・区間取得、全区間取得、セグメント木上の二分探索に対応する汎用セグメント木。
- [UnionFind](libraries/DataStructure/UnionFind.cpp)：集合の統合、連結判定、代表元・サイズ・集合数・所属要素一覧の取得に対応する素集合データ構造。

## Graph

- [Dijkstra](libraries/Graph/Dijkstra.cpp)：非負重みグラフの最短距離計算、頂点列・辺列の経路復元に対応。
- [WarshallFloyd](libraries/Graph/WarshallFloyd.cpp)：全点対最短距離計算、負閉路の検出、頂点列・辺列の経路復元に対応。

## Math

- [ModInt](libraries/Math/ModInt.cpp)：固定MODの四則演算、累乗、逆元、入出力に対応する整数型（逆元・除算は法と互いに素な値に限定）。
- [Prime](libraries/Math/Prime.cpp)：素数判定・列挙・個数、素因数分解、約数、Eulerのφ関数、Möbius関数に対応。

## String

- [RunLengthEncoding](libraries/String/RunLengthEncoding.cpp)：文字列・配列のランレングス圧縮と復元を行う関数群。
