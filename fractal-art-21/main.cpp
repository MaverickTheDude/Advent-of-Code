#include "matrix.h"
using std::string;
using std::cout;
using std::endl;
const int Nlines = 108;
const int Niters = 5; // wersja B (Niters=18) wymaga wylaczenia komentarzy w liniach 45 i w Matrix::enhance

void copyToImage(int idx_i, int idx_j, const Matrix& matrix, array2d<int>& image);
array2d<int> enhanceImage(const array2d<int>& image, const std::vector<pattern>& patternList);


int main() {
    string line;
    std::ifstream myfile("input.txt");
    std::smatch wynik;
    std::regex wzorzec("(.+) => (.+)");
    std::vector<pattern> patternList;
    patternList.reserve(Nlines);

    while( getline(myfile, line) ) {
        std::regex_search(line, wynik, wzorzec);
        std::pair<std::string, std::string> arg = std::make_pair(wynik[1], wynik[2]);
        patternList.emplace_back(arg);
    }
    

    /* prepare input image matrix */
    int imageSize = 3;
    Matrix initMatrix = Matrix();               // initialize entry matrix
    array2d<int> image;                         // - initilize image matrix
    image.resize(imageSize);                    // |
    for (auto& x : image) x.resize(imageSize);  // |
    copyToImage(0, 0, initMatrix, image);       // - copy init matrix to image

    /* core loop */
    for (int i = 0; i < Niters; i++)
        image = enhanceImage(image, patternList);
    
    /* print results */
    int cnt = 0;
    for (int i = 0; i < image.size(); ++i) {
        for (int j = 0; j < image.size(); ++j) {
            cout << image[i][j] << " ";
            if (image[i][j] == 1) cnt++;
        }
        cout << endl;
    }

    std::cout << "done!\n# of active pixels: " << cnt  << std::endl;
    return 0;
}


void copyToImage(int idx_i, int idx_j, const Matrix& matrix, array2d<int>& image) {
    const int& Msize = matrix.Msize;
    for (int i = 0; i < Msize; i++)
        for (int j = 0; j < Msize; j++)
            image[i + idx_i*Msize][j + idx_j*Msize] = matrix.coefs[i][j];
}

array2d<int> enhanceImage(const array2d<int>& image, const std::vector<pattern>& patternList) {
    /* find right indices for the image-matrix depending on the old image-matrix  */
    int Ni, Nj;                                  // # matrices along i, j dimentions
    int matrixSize, oldImageSize, newImageSize;  // block matrix + old & new image-matrix sizes
    oldImageSize = image.size();
    if (oldImageSize % 2 == 0) {       // even: parzysty (2 -> 3)
        matrixSize = 2;
        newImageSize = oldImageSize * 3 / 2;
        Ni = Nj = oldImageSize / 2;
    }
    else {                             // odd: (3 -> 4)
        matrixSize = 3;
        newImageSize = oldImageSize * 4 / 3;
        Ni = Nj = oldImageSize / 3;
    }

    array2d<int> image_new;
    image_new.resize(newImageSize);
    for (auto& x : image_new) x.resize(newImageSize);

    for (int i = 0; i < Ni; i++) {
        for (int j = 0; j < Nj; j++) {
            Matrix Old(i, j, matrixSize, image);
            const Matrix New = Old.enhance(patternList);
            copyToImage(i, j, New, image_new);
        }
    }

    return image_new;
}