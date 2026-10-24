#include <bits/stdc++.h>
using namespace std;

struct IntervalSet {
private:
    set<pair<long long, long long>> data;
    long long covered_length_value;

public:
    // 初期化: IntervalSet() -> 空の整数区間集合を生成
    IntervalSet() : covered_length_value(0) {}

    // 区間追加: insert(l, r) -> [l, r) を追加し、新しく加わった整数の個数を返す
    // 前提: l <= r、更新後の登録整数の総数が long long で表現可能
    long long insert(long long l, long long r) {
        assert(l <= r);
        if (l == r) return 0;

        auto it = data.lower_bound({l, LLONG_MIN});
        if (it != data.begin() && prev(it)->second >= l) --it;
        auto first = it;

        long long new_l = l, new_r = r;
        __int128 erased_length = 0;

        // 重なる区間・隣接する区間をまとめて1区間にする
        while (it != data.end() && it->first <= new_r) {
            new_l = min(new_l, it->first);
            new_r = max(new_r, it->second);
            erased_length += (__int128)it->second - it->first;
            ++it;
        }

        __int128 added = (__int128)new_r - new_l - erased_length;
        assert(0 <= added && (__int128)covered_length_value + added <= LLONG_MAX);

        data.erase(first, it);
        data.emplace(new_l, new_r);
        covered_length_value += (long long)added;
        return (long long)added;
    }

    // 区間削除: erase(l, r) -> [l, r) を削除し、実際に取り除いた整数の個数を返す
    // 前提: l <= r
    long long erase(long long l, long long r) {
        assert(l <= r);
        if (l == r) return 0;

        auto it = data.lower_bound({l, LLONG_MIN});
        if (it != data.begin() && prev(it)->second > l) --it;
        auto first = it;

        __int128 removed = 0;
        optional<pair<long long, long long>> left_part, right_part;

        // 削除対象との共通部分を取り除き、残る両端を保持する
        while (it != data.end() && it->first < r) {
            long long a = it->first, b = it->second;
            long long x = max(l, a), y = min(r, b);
            if (x < y) removed += (__int128)y - x;
            if (a < l) left_part = pair<long long, long long>{a, l};
            if (r < b) right_part = pair<long long, long long>{r, b};
            ++it;
        }

        data.erase(first, it);
        if (left_part) data.insert(*left_part);
        if (right_part) data.insert(*right_part);
        covered_length_value -= (long long)removed;
        return (long long)removed;
    }

    // 全削除: clear() -> 登録されている全区間を削除
    void clear() {
        data.clear();
        covered_length_value = 0;
    }

    // 整数包含判定: contains(x) -> x が登録されていれば true
    bool contains(long long x) const {
        return find(x).has_value();
    }

    // 区間包含判定: contains(l, r) -> [l, r) の全整数が登録されていれば true
    // 前提: l <= r (空区間は常に true)
    bool contains(long long l, long long r) const {
        assert(l <= r);
        if (l == r) return true;
        auto interval = find(l);
        return interval && r <= interval->second;
    }

    // 所属区間取得: find(x) -> x を含む [l, r)、存在しなければ nullopt
    optional<pair<long long, long long>> find(long long x) const {
        auto it = data.upper_bound({x, LLONG_MAX});
        if (it == data.begin()) return nullopt;
        --it;
        if (x >= it->second) return nullopt;
        return *it;
    }

    // 未登録整数検索: mex(x) -> x 以上で登録されていない最小の整数
    long long mex(long long x) const {
        auto interval = find(x);
        return interval ? interval->second : x;
    }

    // 区間数取得: size() -> 登録されている区間の個数
    int size() const {
        return (int)data.size();
    }

    // 空集合判定: empty() -> 登録区間が1つもなければ true
    bool empty() const {
        return data.empty();
    }

    // 登録整数の総数取得: covered_length() -> 各区間の長さの合計
    long long covered_length() const {
        return covered_length_value;
    }

    // 指定範囲の登録整数数取得: covered_length(l, r) -> [l, r) 内の登録整数数
    // 前提: l <= r
    long long covered_length(long long l, long long r) const {
        assert(l <= r);
        if (l == r) return 0;

        auto it = data.lower_bound({l, LLONG_MIN});
        if (it != data.begin() && prev(it)->second > l) --it;

        __int128 result = 0;
        // 対象範囲に重なる登録区間のみ走査する
        while (it != data.end() && it->first < r) {
            long long left = max(l, it->first);
            long long right = min(r, it->second);
            if (left < right) result += (__int128)right - left;
            ++it;
        }
        return (long long)result;
    }

    // 共通部分取得: intersections(l, r) -> [l, r) と登録区間の共通部分一覧
    // 前提: l <= r（左端の昇順、半開区間）
    vector<pair<long long, long long>> intersections(long long l, long long r) const {
        assert(l <= r);
        vector<pair<long long, long long>> result;
        if (l == r) return result;

        auto it = data.lower_bound({l, LLONG_MIN});
        if (it != data.begin() && prev(it)->second > l) --it;

        // 各登録区間を [l, r) で切り取り、空でない部分のみ取得する
        while (it != data.end() && it->first < r) {
            long long left = max(l, it->first);
            long long right = min(r, it->second);
            if (left < right) result.emplace_back(left, right);
            ++it;
        }
        return result;
    }

    // 未登録部分取得: gaps(l, r) -> [l, r) 内の未登録区間一覧
    // 前提: l <= r（左端の昇順、半開区間）
    vector<pair<long long, long long>> gaps(long long l, long long r) const {
        assert(l <= r);
        vector<pair<long long, long long>> result;
        if (l == r) return result;

        auto it = data.lower_bound({l, LLONG_MIN});
        if (it != data.begin() && prev(it)->second > l) --it;

        long long current = l;
        // 登録区間の直前にある空きを列挙する
        while (it != data.end() && it->first < r) {
            long long left = max(l, it->first);
            long long right = min(r, it->second);
            if (current < left) result.emplace_back(current, left);
            if (current < right) current = right;
            ++it;
        }
        if (current < r) result.emplace_back(current, r);
        return result;
    }

    // 全区間取得: intervals() -> [l, r) の一覧を左端昇順で返す
    vector<pair<long long, long long>> intervals() const {
        return vector<pair<long long, long long>>(data.begin(), data.end());
    }
};
