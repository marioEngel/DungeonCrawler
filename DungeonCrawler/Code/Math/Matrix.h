#pragma once
#include <vector>
#include <SDL3/SDL.h>

using Matrix = std::vector<std::vector<int>>;

void printMatrix(Matrix& matrix);
void printMatrixColor(Matrix& matrix);
void makeEmptyMatrix(Matrix& matrix, int collumNr, int rowNr);
void makeChessBoard(Matrix& matrix, int collumNr, int rowNr);
Matrix calcCollisionMatrix(Matrix& matrix); // works only for 0 and 1 and squares

void makeRandomLevel();
// ==============================================================
bool CompareRect(const SDL_Rect& first, const SDL_Rect& second);