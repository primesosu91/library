# library

競技プログラミング向けの自作C++ライブラリです。GNU++17を基本とし、ACLや外部マクロには依存しません。
ライブラリ本体は `libraries/` 以下のカテゴリ別フォルダに配置します。

## DataStructure

- [SegmentTree](libraries/DataStructure/SegmentTree.cpp)：点更新、点・区間取得、全区間取得、セグメント木上の二分探索に対応する汎用セグメント木。
- [LazySegmentTree](libraries/DataStructure/LazySegmentTree.cpp)：区間更新・点更新、点・区間・全区間取得、遅延伝播付き二分探索に対応する汎用遅延セグメント木。
- [UnionFind](libraries/DataStructure/UnionFind.cpp)：集合の統合、連結判定、代表元・サイズ・集合数・所属要素一覧の取得に対応する素集合データ構造。
- [IntervalSet](libraries/DataStructure/IntervalSet.cpp)：整数区間の追加・削除・統合・分割、包含判定、mex・被覆整数数の取得に対応する区間集合。

## Graph

- [Dijkstra](libraries/Graph/Dijkstra.cpp)：非負重みグラフの最短距離計算、頂点列・辺列の経路復元に対応。
- [WarshallFloyd](libraries/Graph/WarshallFloyd.cpp)：全点対最短距離計算、負閉路の検出、頂点列・辺列の経路復元に対応。
- [Doubling](libraries/Graph/Doubling.cpp)：関数グラフ上の繰り返し遷移を高速取得。重みを指定すれば通過頂点の重みの和も取得可能（遷移終了に対応）。
- [CycleDetection](libraries/Graph/CycleDetection.cpp)：有向・無向グラフのサイクル検出、頂点列・辺列の復元に対応（自己ループ・多重辺を含む）。
- [MaxFlow](libraries/Graph/MaxFlow.cpp)：Dinic法による最大流・最小カット計算、辺の流量取得・変更に対応。
- [MinCostFlow](libraries/Graph/MinCostFlow.cpp)：ポテンシャル付き最短増加路法による最小費用流・費用曲線、流量の分割追加と辺の流量取得に対応。

## Math

- [ModInt](libraries/Math/ModInt.cpp)：固定MODの四則演算、累乗、逆元、入出力に対応する整数型（逆元・除算は法と互いに素な値に限定）。
- [Prime](libraries/Math/Prime.cpp)：素数判定・列挙・個数、素因数分解、約数、Eulerのφ関数、Möbius関数に対応。

## String

- [RunLengthEncoding](libraries/String/RunLengthEncoding.cpp)：文字列・配列のランレングス圧縮と復元を行う関数群。
