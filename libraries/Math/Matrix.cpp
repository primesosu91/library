#include <bits/stdc++.h>
using namespace std;

// Tは0・1から構築でき、使用する演算に応じて加算・減算・乗算が可能であること。
// 演算の中間値がTの範囲内に収まること。添字は0-index。
template<class T>
struct Matrix {
private:
    int height, width;
    vector<vector<T>> data;

public:
    // 初期化: Matrix(h,w,value) -> h行w列をvalueで埋める (前提: h,w >= 0)
    Matrix(int h, int w, T value = T(0)) : height(h), width(w) {
        assert(h >= 0 && w >= 0);
        data.assign(h, vector<T>(w, value));
    }

    // 配列から構築: Matrix(a) -> aのコピー (前提: 長方形、行数・列数はintの範囲内)
    // 空配列は0行0列として扱う。
    Matrix(const vector<vector<T>>& a) {
        assert(a.size() <= (size_t)numeric_limits<int>::max());
        assert(a.empty() || a[0].size() <= (size_t)numeric_limits<int>::max());
        height = (int)a.size();
        width = a.empty() ? 0 : (int)a[0].size();
        for (const auto& row : a) {
            assert(row.size() == (size_t)width);
            (void)row;
        }
        data = a;
    }

    // 行数取得: rows() -> 行数
    int rows() const { return height; }

    // 列数取得: cols() -> 列数
    int cols() const { return width; }

    // 行取得: a[i] -> i行目への参照 (前提: 0 <= i < rows(); 行の長さを変更しない)
    // a[i][j]の列添字は0 <= j < cols()を満たすこと。
    vector<T>& operator[](int i) {
        assert(0 <= i && i < height);
        return data[i];
    }

    // 行取得(const): a[i] -> i行目へのconst参照 (前提: 0 <= i < rows())
    const vector<T>& operator[](int i) const {
        assert(0 <= i && i < height);
        return data[i];
    }

    // 単位行列: identity(n) -> n次単位行列 (前提: n >= 0)
    static Matrix identity(int n) {
        Matrix result(n, n);
        for (int i = 0; i < n; i++) result.data[i][i] = T(1);
        return result;
    }

    // 加算代入: a += b -> aにbを加え自身の参照を返す (前提: 同じ行数・列数)
    Matrix& operator+=(const Matrix& other) {
        assert(height == other.height && width == other.width);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) data[i][j] += other.data[i][j];
        }
        return *this;
    }

    // 減算代入: a -= b -> aからbを引き自身の参照を返す (前提: 同じ行数・列数)
    Matrix& operator-=(const Matrix& other) {
        assert(height == other.height && width == other.width);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) data[i][j] -= other.data[i][j];
        }
        return *this;
    }

    // 加算: a + b -> 和を返す (前提: 同じ行数・列数)
    Matrix operator+(const Matrix& other) const {
        Matrix result = *this;
        result += other;
        return result;
    }

    // 減算: a - b -> 差を返す (前提: 同じ行数・列数)
    Matrix operator-(const Matrix& other) const {
        Matrix result = *this;
        result -= other;
        return result;
    }

    // 行列積: a * b -> 積を返す (前提: a.cols() == b.rows())
    Matrix operator*(const Matrix& other) const {
        assert(width == other.height);
        Matrix result(height, other.width);
        // i,k,jの順に走査し、右行列と結果の行を連続して参照する。
        for (int i = 0; i < height; i++) {
            for (int k = 0; k < width; k++) {
                for (int j = 0; j < other.width; j++) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    // 乗算代入: a *= b -> 積をaに代入し自身の参照を返す (前提: a.cols() == b.rows())
    Matrix& operator*=(const Matrix& other) {
        *this = *this * other;
        return *this;
    }

    // 累乗: pow(k) -> 自身のk乗を返す (前提: 正方行列、k >= 0; 0乗は単位行列)
    Matrix pow(long long k) const {
        assert(height == width && k >= 0);
        Matrix result = identity(height);
        Matrix base = *this;
        // 二分累乗。最後の不要な二乗は行わず、余分なオーバーフローを避ける。
        while (k > 0) {
            if (k & 1) result *= base;
            k >>= 1;
            if (k > 0) base *= base;
        }
        return result;
    }
};
