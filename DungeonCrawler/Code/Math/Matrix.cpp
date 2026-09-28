#include "Matrix.h"
#include <iostream>
#include <random>
#include "../Misc/MiscFunctions.h"

void matrixPrint(const Matrix<int>& matrix)
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
				std::cout << matrix(row, col) << " ,";
			}
		}
	}
	std::cout << ' ' << std::endl;
}


void matrixPrintColor(const Matrix<int>& matrix)
{
	for (size_t row = 0; row < matrix.rows(); row++)
	{
		std::cout << "| ";
		for (size_t col = 0; col < matrix.cols(); col++)
		{
			if (col == (matrix.cols() - 1))
			{
				int matVal = matrix(row, col);
				std::string appendString = " |\n";

				switch (matVal)
				{
				case 0:
					std::cout << getColoredText("00", eConsoleTextColor::GREY) << appendString;
					break;
				case 1:
					std::cout << getColoredText("11", eConsoleTextColor::RED) << appendString;
					break;
				case 2:
					std::cout << getColoredText("22", eConsoleTextColor::BLUE) << appendString;
					break;
				case 3:
					std::cout << getColoredText("33", eConsoleTextColor::GREEN) << appendString;
					break;
				case 4:
					std::cout << getColoredText("44", eConsoleTextColor::PURPLE) << appendString;
					break;
				case 5:
					std::cout << getColoredText("55", eConsoleTextColor::ORANGE) << appendString;
					break;
				case 6:
					std::cout << getColoredText("66", eConsoleTextColor::CYAN) << appendString;
					break;
				case 7:
					std::cout << getColoredText("77", eConsoleTextColor::YELLOW) << appendString;
					break;
				case 8:
					std::cout << getColoredText("88", eConsoleTextColor::LIGHT_GREEN) << appendString;
					break;
				case 9:
					std::cout << getColoredText("99", eConsoleTextColor::LIGHT_RED) << appendString;
					break;
				default:
					std::cout << getColoredText("-1", eConsoleTextColor::WHITE) << appendString;
					break;
				}
			}
			else
			{
				int matVal = matrix(row, col);
				std::string appendString = ", ";

				switch (matVal)
				{
				case 0:
					std::cout << getColoredText("00", eConsoleTextColor::GREY) << appendString;
					break;
				case 1:
					std::cout << getColoredText("11", eConsoleTextColor::RED) << appendString;
					break;
				case 2:
					std::cout << getColoredText("22", eConsoleTextColor::BLUE) << appendString;
					break;
				case 3:
					std::cout << getColoredText("33", eConsoleTextColor::GREEN) << appendString;
					break;
				case 4:
					std::cout << getColoredText("44", eConsoleTextColor::PURPLE) << appendString;
					break;
				case 5:
					std::cout << getColoredText("55", eConsoleTextColor::ORANGE) << appendString;
					break;
				case 6:
					std::cout << getColoredText("66", eConsoleTextColor::CYAN) << appendString;
					break;
				case 7:
					std::cout << getColoredText("77", eConsoleTextColor::YELLOW) << appendString;
					break;
				case 8:
					std::cout << getColoredText("88", eConsoleTextColor::LIGHT_GREEN) << appendString;
					break;
				case 9:
					std::cout << getColoredText("99", eConsoleTextColor::LIGHT_RED) << appendString;
					break;
				default:
					std::cout << getColoredText("-1", eConsoleTextColor::WHITE) << appendString;
					break;
				}
			}
		}
	}
	std::cout << ' ' << std::endl;
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

void matrixFillWithElement(Matrix<int>& matrix, int element)
{
	for (int row = 0; row < matrix.rows(); row++)
	{
		for (int col = 0; col < matrix.cols(); col++)
		{
			matrix(row, col) = element;
		}
	}
}

void matrixFillRectWithElement(Matrix<int>& matrix, SDL_Rect& rect, int element)
{
	{
		for (int row = 0; row < matrix.rows(); row++)
		{
			for (int col = 0; col < matrix.cols(); col++)
			{
				if (col >= rect.x && col < rect.x + rect.w &&
					row >= rect.y && row < rect.y + rect.h)
				{
					matrix(row, col) = element;
				}
			}
		}
	}
}