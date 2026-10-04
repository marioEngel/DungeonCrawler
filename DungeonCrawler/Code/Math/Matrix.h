#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "Vector2D.h"

//void matrixCreate_Zero(Matrix& matrix, SDL_Rect& size);
template<typename T>
class Matrix {
public:
    Matrix(int xCols, int yRows) : mXcol(xCols),
                                   mYrow(yRows),
                                   mData(xCols * yRows) {
    }

    explicit Matrix(int square) : Matrix(square, square) {
    }

    [[nodiscard]] T index(int x, int y) const {
        assert(x >= 0 && x < mXcol && y >= 0 && y < mYrow);
        return static_cast<T>(y) * mXcol + x;
    }

    T &operator()(int xCol, int yRow) {
        return mData[index(xCol, yRow)];
    }

    const T &operator()(int xCol, int yRow) const {
        return mData[index(xCol, yRow)];
    }

    T &operator[](const Vector2D<int> &vec) {
        return mData[index(vec.x, vec.y)];
    }

    const T &operator[](const Vector2D<int> &vec) const {
        return mData[index(vec.x, vec.y)];
    }


    [[nodiscard]] int width() const { return mXcol; }
    [[nodiscard]] int height() const { return mYrow; }

private:
    int mXcol, mYrow;
    std::vector<T> mData;
};

//void matrixCreate_Element(Matrix& matrix, SDL_Rect& size, int element);
//
//void matrixFillEdge(Matrix& matrix, int element);
void matrixFillWithElement(Matrix<int> &matrix, int element);

void matrixFillRectWithElement(Matrix<int> &matrix, SDL_Rect &rect, int element);

void matrixPrint(const Matrix<int> &matrix);

void matrixPrintColor(const Matrix<int> &matrix);

void matrixCreate_Checkerboard(Matrix<int> &matrix);
