#include "Matrix.h"
#include <iostream>
#include <random>

void printMatrix(Matrix& matrix)
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		std::cout << "| ";
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (j == (matrix[i].size() - 1))
			{
				std::cout << matrix[i][j] << " |\n";
			}
			else
			{
				std::cout << matrix[i][j] << ", ";
			}
		}
	}
}

void printMatrixColor(Matrix& matrix)
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		std::cout << "| ";
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			if (j == (matrix[i].size() - 1))
			{
				if (matrix[i][j] == 0)
				{
					std::cout << "\033[1;31m0\033[0m  |\n";
				}
				else if (matrix[i][j] == 1)
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
				if (matrix[i][j] == 0)
				{
					std::cout << "\033[1;31m0\033[0m, ";
				}
				else if(matrix[i][j] == 1)
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
void makeEmptyMatrix(Matrix& matrix, int collumNr, int rowNr)
{
	for (size_t i = 0; i < collumNr; i++)
	{
		std::vector<int> tmpVec{};
		for (size_t j = 0; j < rowNr; j++)
		{
			tmpVec.push_back((i + j) % 2);
		}
		matrix.push_back(tmpVec);
	}
}

