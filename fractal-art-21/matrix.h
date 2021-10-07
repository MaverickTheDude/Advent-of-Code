#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

using std::string;
using std::cout;
using std::endl;
template <typename T>
using array2d = std::vector<std::vector<T>>;

class pattern;

class Matrix {
/* 2x2, 3x3 or 4x4 matrix, depending on input */
public:
    Matrix();
    Matrix(const Matrix& M);
    Matrix(const std::string& pattern);
    Matrix(const Matrix& M, int quarter);
    Matrix(int i, int j, int size, const array2d<int>& image);
    ~Matrix();
    Matrix operator=(const Matrix& M) = delete;    // note: Matrix a = b is equivalent to Matrix a(b) => copy ctor is called
    Matrix enhance(const std::vector<pattern>& patternList);
    void rotate();
    void flipH();
    void flipV();
    bool operator==(const Matrix& M) const;
    void print() const;

// private:
    const int Msize;
    bool** coefs;
};

class pattern {
public:
    pattern(std::pair<std::string, std::string> _pattern_) : first(_pattern_.first), second(_pattern_.second) { }

public:
    const Matrix first;
    const Matrix second;
};