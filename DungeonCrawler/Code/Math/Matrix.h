#pragma once
#include <vector>
#include <SDL3/SDL.h>

using Matrix = std::vector<std::vector<int>>;

void printMatrix(Matrix& matrix);
void printMatrixColor(Matrix& matrix);
void makeEmptyMatrix(Matrix& matrix, int collumNr, int rowNr);
