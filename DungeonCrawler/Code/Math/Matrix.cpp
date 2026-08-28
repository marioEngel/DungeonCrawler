#include "Matrix.h"
#include <iostream>
#include <random>

void matrixPrint(Matrix<int>& matrix)
{
	for (size_t row = 0; row < matrix.rows(); row++)
	{
		std::cout << "| ";
		for (size_t col = 0; col < matrix.cols(); col++)
		{
			if (col == (matrix.cols() - 1))
			{
				std::cout << matrix(row, col) << " |\n";
			}
			else
			{
				std::cout << matrix(row, col) << " |\n";
			}
		}
	}
}

void matrixPrintColor(Matrix<int>& matrix)
{
	for (size_t row = 0; row < matrix.rows(); row++)
	{
		std::cout << "| ";
		for (size_t col = 0; col < matrix.cols(); col++)
		{
			if (col == (matrix.cols() - 1))
			{
				if (matrix(row, col) == 0)
				{
					std::cout << "\033[1;31m0\033[0m  |\n";
				}
				else if (matrix(row, col) == 1)
				{
					std::cout << "\033[1;34m1\033[0m  |\n";
				}
				else
				{
					std::cout << "\033[1;32m2\033[0m  |\n";
				}
			}
			else
			{
				if (matrix(row, col) == 0)
				{
					std::cout << "\033[1;31m0\033[0m, ";
				}
				else if (matrix(row, col) == 1)
				{
					std::cout << "\033[1;34m1\033[0m, ";
				}
				else
				{
					std::cout << "\033[1;32m2\033[0m, ";
				}
			}
		}
	}
}

// function to initialize a matrix with the wanted size/shape
// chess board structur
void matrixCreate_Checkboard(Matrix<int>& matrix)
{
	for (size_t row = 0; row < matrix.rows(); row++)
	{
		for (size_t col = 0; col < matrix.cols(); col++)
		{
			matrix(row, col) = (row + col) % 2;
		}
	}
}

//void matrixCreate_Zero(Matrix& matrix, SDL_Rect& size)
//{
//	for (int row = 0; row < size.w; row++)
//	{
//		std::vector<int> tmpVec{};
//		for (int col = 0; col < size.h; col++)
//		{
//
//			tmpVec.push_back(0);
//		}
//		matrix.push_back(tmpVec);
//	}
//}
//
//void matrixCreate_Element(Matrix& matrix, SDL_Rect& size, int element)
//{
//	for (int row = 0; row < size.w; row++)
//	{
//		std::vector<int> tmpVec{};
//		for (int col = 0; col < size.h; col++)
//		{
//
//			tmpVec.push_back(element);
//		}
//		matrix.push_back(tmpVec);
//	}
//}
//
//void matrixFillEdge(Matrix& matrix, int element)
//{
//	for (size_t i = 0; i < matrix.size(); i++)
//	{
//		for (int j = 0; j < matrix[i].size(); j++)
//		{
//			if (i == 0 || i == matrix.size() - 1)
//			{
//				matrix[i][j] = element;
//			}
//			else if (j == 0 || j == matrix[i].size() - 1)
//			{
//				matrix[i][j] = element;
//			}
//		}
//	}
//}