#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "Vector2D.h"

// Matrix mat
// mat[row][col] = ele
//using Matrix = std::vector<std::vector<int>>;

template <typename T>
class Matrix {
public:
    Matrix(int rows, int cols) : 
        mRows(rows), 
        mCols(cols), 
        mData(rows * cols) 
    { }

    Matrix(int square) :
        mRows(square),
        mCols(square),
        mData(square * square)
    { }

    T& operator()(int row, int col) {
        return mData[row * mCols + col];
    } 

    const T& operator()(int row, int col) const {
        return mData[row * mCols + col];
    }

    // Für Vector2D-Zugriff
    T& operator[](const Vector2D<int>& pos) {
        return mData[pos[1] * mCols + pos[0]];
    }

    int rows() const { return mRows; }
    int cols() const { return mCols; }

private:
    int mRows, mCols;
    std::vector<T> mData;
};




void matrixPrint(Matrix<int>& matrix);
void matrixPrintColor(Matrix<int>& matrix);

void matrixCreate_Checkboard(Matrix<int>& matrix);

//void matrixCreate_Zero(Matrix& matrix, SDL_Rect& size);
//void matrixCreate_Element(Matrix& matrix, SDL_Rect& size, int element);
//
//void matrixFillEdge(Matrix& matrix, int element);
