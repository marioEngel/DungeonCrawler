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

void makeChessBoard(Matrix& matrix, int collumNr, int rowNr)
{

}

void makeRandomLevel()
{
	Matrix tmpMatrix;
	makeEmptyMatrix(tmpMatrix, 50, 50);

	std::default_random_engine generator;
	std::normal_distribution<double> distribution(8., 2.0);
	std::uniform_int_distribution<int> intribution(0, 49);

	std::random_device rd;
	generator.seed(rd());

	int roomCount = round(distribution(generator));
	std::vector<int> roomSizes;
	for (int i = 0; i < roomCount; i++)
	{
		roomSizes.push_back(round(distribution(generator)));
	}
	for (int i = 0; i < roomCount; i++)
	{
		if (roomSizes[i] < 3)
		{
			roomSizes[i] = 3;
		}
		else if (roomSizes[i] > 17)
		{
			roomSizes[i] = 17;
		}
	}

	std::cout << "Room count:" << roomCount << std::endl;
	std::cout << "Room sizes:" << std::endl;
	for (int i = 0; i < roomCount; i++)
	{
		std::cout << roomSizes[i] << std::endl;
	}

	int loopValue = 0;
	do
	{
		int xVal = intribution(generator);
		int yVal = intribution(generator);

		if (xVal + roomSizes[loopValue] < 50 && yVal + roomSizes[loopValue] < 50)
		{
			if (tmpMatrix[xVal][yVal] == 0
				&& tmpMatrix[xVal + roomSizes[loopValue]][yVal + roomSizes[loopValue]] == 0
				&& tmpMatrix[xVal][yVal + roomSizes[loopValue]] == 0
				&& tmpMatrix[xVal + roomSizes[loopValue]][yVal] == 0)
			{
				for (size_t i = xVal; i < xVal + roomSizes[loopValue]; i++)
				{
					for (size_t j = yVal; j < yVal + roomSizes[loopValue]; j++)
					{
						tmpMatrix[i][j] = 1;
					}
				}
				loopValue++;
			}
		}
	} while (loopValue < roomCount);
	printMatrixColor(tmpMatrix);
	std::cout << "finished" << std::endl;
}

Matrix calcCollisionMatrix(Matrix& matrix)
{
	Matrix rtnMatrix;
	int tmpSize = matrix.size();
	makeEmptyMatrix(rtnMatrix, tmpSize, tmpSize);
	for (size_t i = 0; i < tmpSize * tmpSize; i++)
	{	
		// top row
		if (i/tmpSize == 0)
		{
			// top left
			if (i == 0)
			{
				if (matrix[i % tmpSize + 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize][i / tmpSize + 1] >= 1)
				{
					if (matrix[i % tmpSize][i / tmpSize] == 0)
					{
						rtnMatrix[i % tmpSize][i / tmpSize] = 1;
					}
				}
			}
			// top right
			else if (i == tmpSize - 1)
			{
				if (matrix[i % tmpSize - 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize][i / tmpSize + 1] >= 1)
				{
					if (matrix[i % tmpSize][i / tmpSize] == 0)
					{
						rtnMatrix[i % tmpSize][i / tmpSize] = 1;
					}
				}
			}
			// rest
			else
			{
				if (matrix[i % tmpSize + 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize - 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize][i / tmpSize + 1] >= 1)
				{
					if (matrix[i % tmpSize][i / tmpSize] == 0)
					{
						rtnMatrix[i % tmpSize][i / tmpSize] = 1;
					}
				}
			}
		}
		// bottom row
		else if (i / tmpSize == tmpSize - 1)
		{
			// bottom left 
			if (i % tmpSize == 0)
			{
				if (matrix[i % tmpSize + 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize][i / tmpSize - 1] >= 1)
				{
					if (matrix[i % tmpSize][i / tmpSize] == 0)
					{
						rtnMatrix[i % tmpSize][i / tmpSize] = 1;
					}
				}
			}
			// bottom right
			else if (i % tmpSize == tmpSize - 1)
			{
				if (matrix[i % tmpSize - 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize][i / tmpSize - 1] >= 1)
				{
					if (matrix[i % tmpSize][i / tmpSize] == 0)
					{
						rtnMatrix[i % tmpSize][i / tmpSize] = 1;
					}
				}
			}
			// rest
			else
			{
				if (matrix[i % tmpSize - 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize + 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize][i / tmpSize - 1] >= 1)
				{
					if (matrix[i % tmpSize][i / tmpSize] == 0)
					{
						rtnMatrix[i % tmpSize][i / tmpSize] = 1;
					}
				}
			}
		}
		//rest
		else
		{
			// left side
			if (i % tmpSize == 0)
			{
				if (matrix[i % tmpSize + 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize][i / tmpSize + 1] >= 1 ||
					matrix[i % tmpSize][i / tmpSize - 1] >= 1)
				{
					if (matrix[i % tmpSize][i / tmpSize] == 0)
					{
						rtnMatrix[i % tmpSize][i / tmpSize] = 1;
					}
				}
			}
			// right side
			else if (i % tmpSize == tmpSize - 1)
			{
				if (matrix[i % tmpSize - 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize][i / tmpSize + 1] >= 1 ||
					matrix[i % tmpSize][i / tmpSize - 1] >= 1)
				{
					if (matrix[i % tmpSize][i / tmpSize] == 0)
					{
						rtnMatrix[i % tmpSize][i / tmpSize] = 1;
					}
				}
			}
			// rest
			else
			{
				if (matrix[i % tmpSize - 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize + 1][i / tmpSize] >= 1 ||
					matrix[i % tmpSize][i / tmpSize + 1] >= 1 ||
					matrix[i % tmpSize][i / tmpSize - 1] >= 1)
				{
					if (matrix[i % tmpSize][i / tmpSize] == 0)
					{
						rtnMatrix[i % tmpSize][i / tmpSize] = 1;
					}
					
				}
			}
		}
	}
	return rtnMatrix;
}

//======================================
// why doesnt sdl have this?
bool CompareRect(const SDL_Rect& first, const SDL_Rect& second)
{
	return first.x == second.x && first.y == second.y
		&& first.h == second.h && first.w == second.w;
}
