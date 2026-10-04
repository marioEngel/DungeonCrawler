#include <iostream>
#include "Math/Vector2D.h"
#include "Math/Matrix.h"


int main(int argc, char *arvg[]) {
    Matrix<int> mat{3};
    Vector2D<int> vec{2, 1};
    Vector2D<int> vec2{0, 2};

    mat[vec] = 1;
    mat[vec2] = 3;

    matrixPrintColor(mat);

    return 0;
}
