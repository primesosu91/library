# library

競技プログラミング向けの自作C++ライブラリです。GNU++17を基本とし、ACLや外部マクロには依存しません。
ライブラリ本体は `libraries/` 以下のカテゴリ別フォルダに配置します。

## DataStructure

- [SegmentTree](libraries/DataStructure/SegmentTree.cpp)：点更新、点・区間取得、全区間取得、セグメント木上の二分探索に対応する汎用セグメント木。
- [LazySegmentTree](libraries/DataStructure/LazySegmentTree.cpp)：区間更新・点更新、点・区間・全区間取得、遅延伝播付き二分探索に対応する汎用遅延セグメント木。
- [UnionFind](libraries/DataStructure/UnionFind.cpp)：集合の統合、連結判定、代表元・サイズ・集合数・所属要素一覧の取得に対応する素集合データ構造。
- [IntervalSet](libraries/DataStructure/IntervalSet.cpp)：整数区間の追加・削除・統合・分割、包含判定、mex・全体および指定範囲の被覆整数数、登録区間との共通部分・未登録区間の取得に対応する区間集合。
- [FenwickTree](libraries/DataStructure/FenwickTree.cpp)：点加算・点代入・点取得・累積和・区間和・累積和に基づく二分探索に対応するFenwick Tree。
- [PrefixSum](libraries/DataStructure/PrefixSum.cpp)：静的配列の1次元累積和を構築し、接頭辞和・半開区間の和・各要素の取得に対応。

## Graph

- [Dijkstra](libraries/Graph/Dijkstra.cpp)：非負重みグラフの最短距離計算、頂点列・辺列の経路復元に対応。
- [BFS](libraries/Graph/BFS.cpp)：単一始点・多点始点の幅優先探索、最短距離計算、頂点列・辺列の経路復元に対応。
- [WarshallFloyd](libraries/Graph/WarshallFloyd.cpp)：全点対最短距離計算、負閉路の検出、頂点列・辺列の経路復元に対応。
- [Doubling](libraries/Graph/Doubling.cpp)：関数グラフ上の繰り返し遷移を高速取得。重みを指定すれば通過頂点の重みの和も取得可能（遷移終了に対応）。
- [CycleDetection](libraries/Graph/CycleDetection.cpp)：有向・無向グラフのサイクル検出、頂点列・辺列の復元に対応（自己ループ・多重辺を含む）。
- [FunctionalGraph](libraries/Graph/FunctionalGraph.cpp)：出次数1の有向グラフを線形時間でサイクル・成分に分解し、到達サイクル・入口・距離、頂点間の到達判定・最短遷移回数・到達可能頂点数を取得。
- [SCC](libraries/Graph/SCC.cpp)：非再帰の強連結成分分解、トポロジカル順の成分ID・成分頂点列・縮約DAGの取得に対応。
- [MaxFlow](libraries/Graph/MaxFlow.cpp)：Dinic法による最大流・最小カット計算、辺の流量取得・変更に対応。
- [MinCostFlow](libraries/Graph/MinCostFlow.cpp)：ポテンシャル付き最短増加路法による最小費用流・費用曲線、流量の分割追加と辺の流量取得に対応。
- [ProjectSelection](libraries/Graph/ProjectSelection.cpp)：二択の利益最大化を最小カットへ帰着。単項・二項利益、全一致ボーナス、禁止状態、解の復元と実行可能性判定に対応。

## Math

- [ModInt](libraries/Math/ModInt.cpp)：固定MODの四則演算、累乗、逆元、入出力に対応する整数型（逆元・除算は法と互いに素な値に限定）。
- [Prime](libraries/Math/Prime.cpp)：素数判定・列挙・個数、素因数分解、約数、Eulerのφ関数、Möbius関数に対応。φ・Möbius関数・約数個数の一括計算にも対応。
- [Combination](libraries/Math/Combination.cpp)：素数MODでの組合せ・順列・重複組合せ、階乗・逆階乗・逆元。前計算範囲外の乗算方式、Lucasの定理、MOD直前の最適化に対応。

## String

- [RunLengthEncoding](libraries/String/RunLengthEncoding.cpp)：文字列・配列のランレングス圧縮と復元を行う関数群。

## Geometry

- [ArgumentSort](libraries/Geometry/ArgumentSort.cpp)：整数座標の偏角ソート、任意の基準点からの比較、外積の符号判定に対応。

## Other

- [CoordinateCompression](libraries/Other/CoordinateCompression.cpp)：静的座標圧縮、値と圧縮座標の相互変換、配列圧縮、未登録値を含む二分探索に対応。
- [LIS](libraries/Other/LIS.cpp)：最長増加部分列（狭義・広義）の長さ、値列・添字列の復元、各位置で終わる最長長さの取得に対応。

## Snippets

- [cpp.code-snippets](snippets/cpp.code-snippets)：VS Code用のC++スニペット。競技用テンプレートと各ライブラリを収録。
