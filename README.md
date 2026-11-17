# library

競技プログラミング向けの自作C++ライブラリです。GNU++17を基本とし、ACLや外部マクロには依存しません。
ライブラリ本体は `libraries/` 以下のカテゴリ別フォルダに配置します。
各項目のリンク先に、公開API・前提条件などの説明コメントがあります。

## DataStructure

- [SegmentTree](libraries/DataStructure/SegmentTree.cpp)：点更新・区間集約・区間に対する二分探索を行う汎用セグメント木。
- [LazySegmentTree](libraries/DataStructure/LazySegmentTree.cpp)：区間更新・区間集約・区間に対する二分探索を行う汎用遅延セグメント木。
- [UnionFind](libraries/DataStructure/UnionFind.cpp)：集合の統合・連結判定・サイズ取得・所属要素の列挙を行う素集合データ構造。
- [IntervalSet](libraries/DataStructure/IntervalSet.cpp)：整数区間の追加・削除、包含判定、mex、被覆整数数の取得を行う区間集合。
- [FenwickTree](libraries/DataStructure/FenwickTree.cpp)：点更新・区間和・累積和に基づく二分探索を行うFenwick Tree。
- [PrefixSum](libraries/DataStructure/PrefixSum.cpp)：静的配列の接頭辞和・区間和を求める1次元累積和。

## Graph

- [MinimumSpanningTree](libraries/Graph/MinimumSpanningTree.cpp)：整数・浮動小数点の重みに対応する最小全域木・最小全域森の構築。
- [Dijkstra](libraries/Graph/Dijkstra.cpp)：非負重みグラフの単一始点最短路と経路復元。
- [BFS](libraries/Graph/BFS.cpp)：重みなしグラフの単一・多点始点最短路と経路復元。
- [WarshallFloyd](libraries/Graph/WarshallFloyd.cpp)：全点対最短路、負閉路検出、経路復元。
- [Doubling](libraries/Graph/Doubling.cpp)：関数グラフ上の繰り返し遷移と、遷移に伴う重みの和の取得。
- [CycleDetection](libraries/Graph/CycleDetection.cpp)：有向・無向グラフのサイクル検出と復元。
- [FunctionalGraph](libraries/Graph/FunctionalGraph.cpp)：関数グラフのサイクル分解、到達判定、最短遷移回数・到達可能頂点数の取得。
- [SCC](libraries/Graph/SCC.cpp)：有向グラフの強連結成分分解と縮約DAGの構築。
- [TopologicalSort](libraries/Graph/TopologicalSort.cpp)：有向グラフのトポロジカルソート、辞書順最小の順序取得、閉路判定。
- [MaxFlow](libraries/Graph/MaxFlow.cpp)：最大流・最小カットの計算と辺ごとの流量取得・変更。
- [MinCostFlow](libraries/Graph/MinCostFlow.cpp)：最小費用流・費用曲線の計算と辺ごとの流量取得。
- [ProjectSelection](libraries/Graph/ProjectSelection.cpp)：二択の利益・制約を最小カットに帰着する最適化と選択結果の復元。

## Tree

- [LCA](libraries/Tree/LCA.cpp)：重みなし木の最小共通祖先、頂点間距離、深さ、祖先の取得。
- [TreeDiameter](libraries/Tree/TreeDiameter.cpp)：重みなし・非負整数重みの木の直径計算と経路復元。

## Math

- [ModInt](libraries/Math/ModInt.cpp)：固定MODでの四則演算・累乗・逆元を扱う整数型。
- [Prime](libraries/Math/Prime.cpp)：素数判定・列挙、素因数分解、約数、Eulerのφ関数・Möbius関数の計算。
- [Combination](libraries/Math/Combination.cpp)：素数MODでの組合せ・順列・重複組合せ、階乗・逆階乗・逆元の計算。
- [Convolution](libraries/Math/Convolution.cpp)：法998244353での畳み込み・多項式乗算（ModInt型を指定して使用）。
- [FPS](libraries/Math/FPS.cpp)：法998244353の形式的冪級数の演算・微積分・対数・指数・累乗（ModInt型とConvolutionを併用）。
- [BaseConversion](libraries/Math/BaseConversion.cpp)：非負整数とN進法の桁列・文字列の相互変換。
- [Matrix](libraries/Math/Matrix.cpp)：任意の要素型による行列の加減算・乗算・累乗。
- [XorBasis](libraries/Math/XorBasis.cpp)：64ビット非負整数のXOR基底、表現可能判定、XOR値の最大・最小値の取得。

## String

- [Trie](libraries/String/Trie.cpp)：文字列の登録・削除・完全一致検索・接頭辞検索を行うTrie木。
- [ZAlgorithm](libraries/String/ZAlgorithm.cpp)：文字列・配列の各位置からの接尾辞と、全体の接頭辞との最長一致長の計算。
- [RunLengthEncoding](libraries/String/RunLengthEncoding.cpp)：文字列・配列のランレングス圧縮と復元。

## Geometry

- [ArgumentSort](libraries/Geometry/ArgumentSort.cpp)：整数座標の偏角ソート、偏角比較、外積の符号判定。

## Other

- [CoordinateCompression](libraries/Other/CoordinateCompression.cpp)：静的座標圧縮、元の値と圧縮座標の相互変換、二分探索。
- [LIS](libraries/Other/LIS.cpp)：狭義・広義の最長増加部分列の長さ計算と復元。
- [DateTime](libraries/Other/DateTime.cpp)：日時の変換・加減算・差分、曜日・うるう年の判定、経過時間の変換。

## Snippets

- [cpp.code-snippets](snippets/cpp.code-snippets)：VS Code用のC++スニペット。競技用テンプレートと各ライブラリを収録。
