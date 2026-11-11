#include <bits/stdc++.h>
using namespace std;

// 連続するバイト値 [base, base + alphabet_size) を扱う文字列の多重集合。
// 文字列長・登録総数・作成済みノード数は int の範囲内。空文字列も登録可能。
// L = 文字列長、σ = alphabet_size、V = 作成済みノード数として、
// insert は償却 O(1 + σL)、検索・削除は O(1 + L)、保持領域は O(σV)。
// 削除時もノードを保持するため、V は過去の挿入履歴に依存する。
template<int alphabet_size = 26, char base = 'a'>
struct Trie {
private:
    static_assert(alphabet_size > 0, "alphabet_size must be positive");
    static_assert(alphabet_size <= UCHAR_MAX - (unsigned char)base + 1,
                  "alphabet must fit in unsigned char");

    struct Node {
        array<int, alphabet_size> next;
        int terminal_count = 0;
        int subtree_count = 0;

        Node() { next.fill(-1); }
    };

    vector<Node> nodes;

    int character_index(char c) const {
        int index = (unsigned char)c - (unsigned char)base;
        assert(0 <= index && index < alphabet_size);
        return index;
    }

    // 終点のノード番号を取得。経路がなければ -1。
    int find_node(const string& s) const {
        assert(s.size() <= (size_t)INT_MAX);
        int v = 0;
        for (char c : s) {
            v = nodes[v].next[character_index(c)];
            if (v == -1) return -1;
        }
        return v;
    }

public:
    // 初期化: Trie() -> 登録件数0のTrieを生成。時間 O(σ)。
    Trie() : nodes(1) {}

    // 追加: insert(s) -> s を1件追加、返り値なし。重複可。
    // 前提: s は対象文字のみで構成され、更新後の登録総数・ノード数が int の範囲内。
    void insert(const string& s) {
        assert(s.size() <= (size_t)INT_MAX);
        assert(size() < INT_MAX);
        int v = 0;
        nodes[v].subtree_count++;
        for (char c : s) {
            int index = character_index(c);
            int to = nodes[v].next[index];
            if (to == -1) {
                assert(nodes.size() < (size_t)INT_MAX);
                to = (int)nodes.size();
                // 再確保をまたいでノードへの参照を保持しない。
                nodes.emplace_back();
                nodes[v].next[index] = to;
            }
            v = to;
            nodes[v].subtree_count++;
        }
        nodes[v].terminal_count++;
    }

    // 削除: erase(s) -> s を1件削除して true。未登録なら変更せず false。
    // 前提: s は対象文字のみで構成される。ノード自体は削除しない。
    bool erase(const string& s) {
        int terminal = find_node(s);
        if (terminal == -1 || nodes[terminal].terminal_count == 0) return false;

        // 完全一致を確認してから経路上の登録件数を減らす。
        nodes[terminal].terminal_count--;
        int v = 0;
        nodes[v].subtree_count--;
        for (char c : s) {
            v = nodes[v].next[character_index(c)];
            nodes[v].subtree_count--;
        }
        return true;
    }

    // 完全一致件数: count(s) -> s の登録件数。未登録なら0。
    // 前提: s は対象文字のみで構成される。空文字列も完全一致で数える。
    int count(const string& s) const {
        int v = find_node(s);
        return v == -1 ? 0 : nodes[v].terminal_count;
    }

    // 包含判定: contains(s) -> s が1件以上登録されているか。
    // 前提: s は対象文字のみで構成される。
    bool contains(const string& s) const { return count(s) > 0; }

    // 接頭辞件数: prefix_count(prefix) -> prefix から始まる登録件数 (重複込み)。
    // 前提: prefix は対象文字のみで構成される。空接頭辞では size() を返す。
    int prefix_count(const string& prefix) const {
        int v = find_node(prefix);
        return v == -1 ? 0 : nodes[v].subtree_count;
    }

    // 最長共通接頭辞: longest_prefix_length(s) -> 登録文字列との共通接頭辞の最大長。
    // 前提: s は対象文字のみで構成される。空集合では0。単語の終端とは無関係。
    int longest_prefix_length(const string& s) const {
        assert(s.size() <= (size_t)INT_MAX);
        int v = 0, length = 0;
        for (char c : s) {
            int to = nodes[v].next[character_index(c)];
            if (to == -1 || nodes[to].subtree_count == 0) break;
            v = to;
            length++;
        }
        return length;
    }

    // 登録総数: size() -> 重複・空文字列を含む登録件数。時間 O(1)。
    int size() const { return nodes[0].subtree_count; }

    // 空集合判定: empty() -> 登録総数が0なら true。時間 O(1)。
    bool empty() const { return size() == 0; }
};
