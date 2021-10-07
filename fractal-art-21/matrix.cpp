#include "matrix.h"

static int setSize(const std::string& pattern) {
    if (pattern.size() == 5)        return 2;
    else if (pattern.size() == 11)  return 3;
    else if (pattern.size() == 19)  return 4;
    else throw(std::runtime_error("Matrix::wrong pattern scheme"));
}

Matrix::Matrix() : Msize(3) {
    /* allocate memmory */
    coefs = new bool*[Msize];
    for (int i = 0; i < Msize; ++i)
        coefs[i] = new bool[Msize];
    
    /* fill in data */
    int cnt = 0;
    bool init[9] = { 0, 0, 1, 1, 0, 1, 0, 1, 1 };
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            coefs[j][i] = init[cnt++];
}

/* NOTE: copy constructor must be specified to copy the data into NEW MEMORY SPACE 
 * (deep copy, not just different pointers to the same memory space) */
Matrix::Matrix(const Matrix& M) : Msize(M.Msize) {
    /* allocate memmory */
    coefs = new bool*[Msize];
    for (int i = 0; i < Msize; ++i)
        coefs[i] = new bool[Msize];

    for (int i = 0; i < Msize; ++i)
        for (int j = 0; j < Msize; ++j)
            coefs[i][j] = M.coefs[i][j];
}

Matrix::Matrix(const std::string& pattern) : Msize(setSize(pattern)) {
    /* allocate memmory */
    coefs = new bool*[Msize];
    for (int i = 0; i < Msize; ++i)
        coefs[i] = new bool[Msize];
    
    /* fill in data */
    int index = 0;
    for (int i = 0; i < Msize; ++i) {
        for (int j = 0; j < Msize; ++j) {
            if (pattern[index] == '/')
                ++index;
            coefs[i][j] = (pattern[index] == '#' ? true : false);
            bool test = coefs[i][j];
            ++index;
        }
    }
}

Matrix::Matrix(const Matrix& M, int quarter) : Msize(2) {
    if (M.Msize != 4)
        throw(std::runtime_error("Matrix::Matrix(Matrix, int): only 4x4 Matrix allowable "));

    int s_i, s_j; // starting coordinates
    switch (quarter) {
        case 1 : s_i = 0; s_j = 2; break;
        case 2 : s_i = 0; s_j = 0; break;
        case 3 : s_i = 2; s_j = 2; break;
        case 4 : s_i = 2; s_j = 2; break;
        default:  throw(std::runtime_error("Matrix::Matrix(Matrix, int): quarter = {1,2,3,4} "));
    }
    /* allocate memmory */
    coefs = new bool*[Msize];
    for (int i = 0; i < Msize; ++i)
        coefs[i] = new bool[Msize];

    /* fill in data */
    for (int i = s_i; i < 2; i++) {
        for (int j = s_j; j < 2; j++)
            coefs[i][j] = M.coefs[i][j];
    }
}

/* copies image fragment at (idx_i,idx_j) of size=2 or size=3 */
Matrix::Matrix(int idx_i, int idx_j, int size, const array2d<int>& image) : Msize(size) {
    /* allocate memmory */
    coefs = new bool*[Msize];
    for (int i = 0; i < Msize; ++i)
        coefs[i] = new bool[Msize];

    /* fill in data */
    for (int i = 0; i < Msize; i++)
        for (int j = 0; j < Msize; j++)
            this->coefs[i][j] = image[i + idx_i*size][j + idx_j*size];
}


Matrix::~Matrix() {
    for (int i = 0; i < Msize; ++i)
        delete coefs[i];

    delete coefs;
}

void Matrix::rotate() {
    /* rotate 90 degrees clockwise */
    if (Msize == 2) {
        const bool tmp = coefs[0][0];
        coefs[0][0] = coefs[1][0];
        coefs[1][0] = coefs[1][1];
        coefs[1][1] = coefs[0][1];
        coefs[0][1] = tmp;
    }
    else if (Msize == 3) {
        const int n = Msize - 1; // last index of the matrix (= 2)
        bool tmp01 = coefs[0][1], tmp02 = coefs[0][2];

        for (int i = 2; i > -1; i--)    coefs[ 0 ][ i ] = coefs[2 - i][0]; // execute 3x
        for (int i = 0; i <  2; i++)    coefs[2 - i][0] = coefs[2][2 - i]; // execute 2x
        coefs[2][1] = coefs[1][2];
        coefs[2][2] = tmp02;
        coefs[1][2] = tmp01;
    }
    else throw(std::runtime_error("Matrix::rotate not implemented"));
}

void Matrix::flipH() {
    if (Msize == 2) {
        const bool tmp1 = coefs[0][0];
        const bool tmp2 = coefs[0][1];
        coefs[0][0] = coefs[1][0];
        coefs[0][1] = coefs[1][1];
        coefs[1][0] = tmp1;
        coefs[1][1] = tmp2;
    }
    else if (Msize == 3) {
        for (int i = 0; i < Msize; i++) {
            const bool tmp = coefs[0][i];
            coefs[0][i] = coefs[2][i];
            coefs[2][i] = tmp;
        }
    }
    else throw(std::runtime_error("Matrix::flipH not implemented"));
}

void Matrix::flipV() {
    if (Msize == 2) {
        const bool tmp1 = coefs[0][0];
        const bool tmp2 = coefs[1][0];
        coefs[0][0] = coefs[0][1];
        coefs[1][0] = coefs[1][1];
        coefs[0][1] = tmp1;
        coefs[1][1] = tmp2;
    }
    else if (Msize == 3) {
        for (int i = 0; i < 3; i++) {
            const bool tmp = coefs[i][0];
            coefs[i][0] = coefs[i][2];
            coefs[i][2] = tmp;
        }
    }
    else throw(std::runtime_error("Matrix::flipV not implemented. Msize = " + std::to_string(Msize)));
}

void Matrix::print() const {
    for (int i = 0; i < Msize; ++i) {
        for (int j = 0; j < Msize; ++j) {
            cout << coefs[i][j] << " ";
        }
        cout << "\n";
    }
        cout << "\n" << endl;
}

bool Matrix::operator==(const Matrix& M) const {
    if (Msize != M.Msize)
        return false;
    for (int i = 0; i < Msize; i++) {
        for (int j = 0; j < Msize; j++) {
            if (coefs[i][j] != M.coefs[i][j])
                return false;
        }
    }
    return true;
}

Matrix Matrix::enhance(const std::vector<pattern>& patternList) {
    int cnt = 0;
    if (Msize < 2 || Msize > 3)
        throw(std::runtime_error("Matrix::enhance: wrong matrix size"));
    /* iterate over all patterns... */
    for (const pattern& _pattern_ : patternList) {
        ++cnt;
        const Matrix& ref = _pattern_.first;
        /* but only the right input size... */
        if (this->Msize != ref.Msize)
            continue;

        /* check all 8 possible configurations... */
        if (ref == *this) {
            std::cout << "rule " << cnt << " activated. configuration id = " << 0 << std::endl;
            return _pattern_.second;
        }

        flipV();
        if (ref == *this) {
            std::cout << "rule " << cnt << " activated. configuration id = " << 1 << std::endl;
            return _pattern_.second;
        }

        flipH();
        if (ref == *this) {
            std::cout << "rule " << cnt << " activated. configuration id = " << 2 << std::endl;
            return _pattern_.second;
        }

        flipV();
        if (ref == *this) {
            std::cout << "rule " << cnt << " activated. configuration id = " << 3 << std::endl;
            return _pattern_.second;
        }

        rotate();
        if (ref == *this) {
            std::cout << "rule " << cnt << " activated. configuration id = " << 4 << std::endl;
            return _pattern_.second;
        }

        flipH();
        if (ref == *this) {
            std::cout << "rule " << cnt << " activated. configuration id = " << 5 << std::endl;
            return _pattern_.second;
        }

        flipV();
        if (ref == *this) {
            std::cout << "rule " << cnt << " activated. configuration id = " << 6 << std::endl;
            return _pattern_.second;
        }
        
        flipH();
        if (ref == *this) {
            std::cout << "rule " << cnt << " activated. configuration id = " << 7 << std::endl;
            return _pattern_.second;
        }
    }

    throw(std::runtime_error("Matrix::enhance: no suitable rule!"));
}