#include <bits/stdc++.h>
using namespace std;
// 年は1〜9999。各APIの時間・追加領域はO(1)。
// うるう年判定: year がグレゴリオ暦のうるう年ならtrue。
bool is_leap_year(int year) {
    assert(1 <= year && year <= 9999);
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

// 月の日数: year年month月の日数を返す (前提: 年が有効、月は1〜12)。
int days_in_month(int year, int month) {
    assert(1 <= year && year <= 9999 && 1 <= month && month <= 12);
    const int lengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return lengths[month - 1] + (month == 2 && is_leap_year(year));
}

// 妥当性判定: 年月日時分秒が対応範囲の有効な日時ならtrue。不正な値も入力可能。
bool is_valid_datetime(int year, int month, int day,
                       int hour = 0, int minute = 0, int second = 0) {
    return 1 <= year && year <= 9999 && 1 <= month && month <= 12
        && 1 <= day && day <= days_in_month(year, month)
        && 0 <= hour && hour < 24 && 0 <= minute && minute < 60
        && 0 <= second && second < 60;
}

// 経過時間を秒へ: hours時間minutes分seconds秒の合計を返す。
// 前提: hours >= 0 (24以上も可)、分・秒は0〜59、合計がlong longに収まる。
long long hms_to_seconds(long long hours, int minutes, int seconds) {
    assert(hours >= 0 && 0 <= minutes && minutes < 60 && 0 <= seconds && seconds < 60);
    int remainder = minutes * 60 + seconds;
    assert(hours <= (LLONG_MAX - remainder) / 3600);
    return hours * 3600 + remainder;
}

// 経過秒数を分解: {時間, 分, 秒}を返す (前提: seconds >= 0)。時間は24以上も可。
tuple<long long, int, int> seconds_to_hms(long long seconds) {
    assert(seconds >= 0);
    return {seconds / 3600, (int)(seconds / 60 % 60), (int)(seconds % 60)};
}

// 0001-01-01 00:00:00〜9999-12-31 23:59:59の日時。月・日は1始まり。
// 全期間でグレゴリオ暦を適用。1日=86400秒、タイムゾーン・夏時間・うるう秒なし。
// 全APIの時間・追加領域はO(1)。演算結果は対応範囲内であること。
struct DateTime {
private:
    long long value;
    static constexpr long long min_seconds = -62135596800LL;
    static constexpr long long max_seconds = 253402300799LL;

    // 3月を年初として400年周期に分け、1970-01-01からの日数に変換する。
    static int days_from_date(int year, int month, int day) {
        year -= month <= 2;
        int era = year / 400;
        int year_of_era = year % 400;
        int shifted_month = month + (month > 2 ? -3 : 9);
        int day_of_year = (153 * shifted_month + 2) / 5 + day - 1;
        return era * 146097 + year_of_era * 365 + year_of_era / 4
             - year_of_era / 100 + day_of_year - 719468;
    }

    // 負の秒数も、その日時が属する日の午前0時まで切り下げる。
    int day_index() const {
        return (int)(value / 86400 - (value % 86400 < 0));
    }

public:
    // 初期化: 年月日時分秒から構築。時刻省略時は午前0時 (前提: 有効な日時)。
    DateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0) {
        assert(is_valid_datetime(year, month, day, hour, minute, second));
        value = (long long)days_from_date(year, month, day) * 86400
              + hour * 3600 + minute * 60 + second;
    }

    // 秒数から構築: 1970-01-01 00:00:00からseconds秒後の日時を返す。
    // 前提: -62135596800 <= seconds <= 253402300799。負数も可。
    static DateTime from_seconds(long long seconds) {
        assert(min_seconds <= seconds && seconds <= max_seconds);
        DateTime result(1970, 1, 1);
        result.value = seconds;
        return result;
    }

    // 秒数取得: 1970-01-01 00:00:00からの符号付き秒数を返す。
    long long to_seconds() const { return value; }

    // 日付取得: {年, 月, 日}を返す。月・日は1始まり。
    tuple<int, int, int> date() const {
        // 400年周期内の日数から年・3月始まりの月・日を復元する。
        int days = day_index() + 719468;
        int era = days / 146097;
        int day_of_era = days % 146097;
        int year_of_era = (day_of_era - day_of_era / 1460
                        + day_of_era / 36524 - day_of_era / 146096) / 365;
        int year = era * 400 + year_of_era;
        int day_of_year = day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100);
        int shifted_month = (5 * day_of_year + 2) / 153;
        int day = day_of_year - (153 * shifted_month + 2) / 5 + 1;
        int month = shifted_month + (shifted_month < 10 ? 3 : -9);
        year += month <= 2;
        return {year, month, day};
    }

    // 時刻取得: {時, 分, 秒}を返す。それぞれ0〜23、0〜59、0〜59。
    tuple<int, int, int> time() const {
        int seconds = (int)((value % 86400 + 86400) % 86400);
        return {seconds / 3600, seconds / 60 % 60, seconds % 60};
    }

    // 曜日取得: 月曜0〜日曜6を返す。
    int weekday() const { return ((day_index() + 3) % 7 + 7) % 7; }

    // 秒数加算: seconds秒後の新しい日時を返す。負数も可、元の日時は変更しない。
    // 前提: 結果が対応範囲内。加算前に差分の範囲を検査する。
    DateTime add_seconds(long long seconds) const {
        assert(min_seconds - value <= seconds && seconds <= max_seconds - value);
        return from_seconds(value + seconds);
    }

    // 日数加算: 同じ時刻のdays日後を返す。負数も可、元の日時は変更しない。
    // 前提: 結果が対応範囲内。乗算前に許容日数を検査する。
    DateTime add_days(long long days) const {
        assert((min_seconds - value) / 86400 <= days && days <= (max_seconds - value) / 86400);
        return add_seconds(days * 86400);
    }

    // 日時差: 自分からotherを引いた符号付き経過秒数を返す。
    long long operator-(const DateTime& other) const { return value - other.value; }

    // 等値比較: otherと同じ日時ならtrue。
    bool operator==(const DateTime& other) const { return value == other.value; }
    // 不等比較: otherと異なる日時ならtrue。
    bool operator!=(const DateTime& other) const { return value != other.value; }
    // 前後比較: otherより前ならtrue。
    bool operator<(const DateTime& other) const { return value < other.value; }
    // 前後比較: otherより前または同じならtrue。
    bool operator<=(const DateTime& other) const { return value <= other.value; }
    // 前後比較: otherより後ならtrue。
    bool operator>(const DateTime& other) const { return value > other.value; }
    // 前後比較: otherより後または同じならtrue。
    bool operator>=(const DateTime& other) const { return value >= other.value; }
};
