# library

競技プログラミング向けの自作C++ライブラリです。GNU++17を基本とし、ACLや外部マクロには依存しません。
ライブラリ本体は `libraries/` 以下のカテゴリ別フォルダに配置します。

## DataStructure

- [SegmentTree](libraries/DataStructure/SegmentTree.cpp)：点更新、点・区間取得、全区間取得、セグメント木上の二分探索に対応する汎用セグメント木。
