#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

bool check_valid_input_line(std::string& strLine, std::vector<int>& oneLine)
{
	if (strLine.length() != 81)
	{
		return false;
	}

	std::replace(strLine.begin(), strLine.end(), '.', '0');

	int test = 0;
	for (size_t i = 0; i < strLine.length(); i++)
	{
		test = strLine[i] - '0';
		if (!(test >= 0 && test <= 9))
		{
			return false;
		}

		oneLine.push_back(test);
	}

	return true;
}

bool read_from_file(const char* fileName, std::vector<std::vector<int>>& allLines)
{
	std::ifstream ifs{ fileName };
	if (!ifs.is_open())
	{
		return false;
	}

	std::string strLine;
	while (std::getline(ifs, strLine))
	{
		std::vector<int> oneLine;

		if (!(check_valid_input_line(strLine, oneLine)))
		{
			return false;
		}

		allLines.push_back(oneLine);
	}

	if (ifs.eof())
	{
		return true;
	}

	return false;
}

bool is_in_row_or_col(const std::vector<std::vector<int>>& sudoku, int num, int row, int col)
{
	for (size_t i = 0; i < sudoku.size(); i++)
	{
		if (sudoku[i][col] == num)
		{
			return true;
		}

		if (sudoku[row][i] == num)
		{
			return true;
		}
	}
	return false;
}

bool is_in_square(const std::vector<std::vector<int>>& sudoku, int num, int row, int col)
{
	int squares = 3;
	int firstRow = (row / squares) * squares;
	int firstCol = (col / squares) * squares;

	for (int i = 0; i < squares; i++)
	{
		for (int j = 0; j < squares; j++)
		{
			if (sudoku[i + firstRow][j + firstCol] == num)
			{
				return true;
			}
		}
	}
	return false;
}

bool is_fit(const std::vector<std::vector<int>>& sudoku, int num, int row, int col)
{
	return ((!is_in_row_or_col(sudoku, num, row, col)) && (!is_in_square(sudoku, num, row, col)));
}

bool is_empty(const std::vector<std::vector<int>>& sudoku, int row, int col)
{
	return (sudoku[row][col] == 0);
}

void print_sudoku_9x9(const std::vector<std::vector<int>>& sudoku)
{
	for (size_t i = 0; i < sudoku.size(); i++)
	{
		for (size_t j = 0; j < sudoku[i].size(); j++)
		{
			std::cout << sudoku[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "------------------------------" << std::endl;
}

bool check_rows(const std::vector<std::vector<int>>& sudoku)
{
	for (size_t row = 0; row < sudoku.size(); row++)
	{
		int rowMap[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		for (size_t col = 0; col < sudoku.size(); col++)
		{
			rowMap[sudoku[row][col]]++;
		}

		for (size_t i = 1; i < 10; i++)
		{
			if (rowMap[i] != 1)
			{
				return false;
			}
		}
	}
	return true;
}

bool check_cols(const std::vector<std::vector<int>>& sudoku)
{
	for (size_t col = 0; col < sudoku.size(); col++)
	{
		int colMap[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		for (size_t row = 0; row < sudoku.size(); row++)
		{
			colMap[sudoku[row][col]]++;
		}

		for (size_t i = 1; i < 10; i++)
		{
			if (colMap[i] != 1)
			{
				return false;
			}
		}
	}
	return true;
}

bool check_squares(const std::vector<std::vector<int>>& sudoku)
{
	for (size_t rowSq = 0; rowSq < 3; rowSq++)
	{
		for (size_t colSq = 0; colSq < 3; colSq++)
		{
			int sqMap[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			for (size_t row = rowSq * 3; row < rowSq * 3 + 3; row++)
			{
				for (size_t col = colSq * 3; col < colSq * 3 + 3; col++)
				{
					sqMap[sudoku[row][col]]++;
				}
			}

			for (size_t i = 1; i < 10; i++)
			{
				if (sqMap[i] != 1)
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool check_sudoku(const std::vector<std::vector<int>>& sudoku)
{
	return (check_rows(sudoku) && check_cols(sudoku) && check_squares(sudoku));
}

bool solve_sudoku_recursive(std::vector<std::vector<int>>& sudoku)
{	
	int row = 0;
	int col = 0;
	bool empty = false;

	for (int id = 0; id < 81; id++)
	{
		row = id / 9;
		col = id % 9;

		if (is_empty(sudoku, row, col))
		{
			empty = true;
			break;
		}
	}

	if (empty)
	{
		for (int num = 1; num <= 9; num++)
		{
			if (is_fit(sudoku, num, row, col))
			{
				sudoku[row][col] = num;
				if (solve_sudoku_recursive(sudoku))
				{
					return true;
				}
				sudoku[row][col] = 0;
			}
		}
	}
	else
	{
		return true;
	}

	return false;
}

int sudoku_solver(const std::vector<int>& oneLineFull)
{
	std::vector<std::vector<int>> sudoku;
	sudoku.resize(9);
	for (size_t i = 0; i < sudoku.size(); i++)
	{
		sudoku[i].resize(9);
	}

	for (size_t i = 0; i < oneLineFull.size(); i++)
	{
		sudoku[i / 9][i % 9] = oneLineFull[i];
	}

	//print_sudoku_9x9(sudoku);

	if (solve_sudoku_recursive(sudoku) && check_sudoku(sudoku))
	{
		//print_sudoku_9x9(sudoku);
		
		for (size_t i = 0; i < sudoku.size(); i++)
		{
			for (size_t j = 0; j < sudoku.size(); j++)
			{
				std::cout << sudoku[i][j];
			}
		}
		std::cout << std::endl;
	}
	else
	{
		std::cout << std::endl;
	}

	return 0;
}

int set_variables(int argc, char* argv[], bool& inputFile, bool& outputFile, const char*& inputFileName, const char*& outputFileName) 
{

	if (argc == 1)
	{
		inputFile = false;
		outputFile = false;
	}
	else if (argc == 3)
	{
		if (std::string(argv[1]) == "-i")
		{
			inputFileName = argv[2];
			inputFile = true;
		}
		else if (std::string(argv[1]) == "-o")
		{
			outputFileName = argv[2];
			outputFile = true;
		}
		else
		{
			return 2;
		}
	}
	else if (argc == 5)
	{
		if (std::string(argv[1]) == "-i")
		{
			inputFileName = argv[2];
			inputFile = true;
		}
		else if (std::string(argv[1]) == "-o")
		{
			outputFileName = argv[2];
			outputFile = true;
		}
		else
		{
			return 2;
		}

		if (std::string(argv[3]) == "-i")
		{
			inputFileName = argv[4];
			inputFile = true;
		}
		else if (std::string(argv[3]) == "-o")
		{
			outputFileName = argv[4];
			outputFile = true;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 2;
	}
	
	return 0;
}

bool read_from_standard()
{
	std::string strLine;
	while (std::getline(std::cin, strLine))
	{
		std::vector<int> oneLine;

		if (!(check_valid_input_line(strLine, oneLine)))
		{
			return false;
		}

		sudoku_solver(oneLine);
	}

	return true;
}

bool sudoku_solver_caller(bool inputFile, const char* inputFileName, std::vector<std::vector<int>>& allLines)
{
	if (inputFile)
	{
		if (!(read_from_file(inputFileName, allLines)))
		{
			return false;
		}

		for (size_t i = 0; i < allLines.size(); i++)
		{
			sudoku_solver(allLines[i]);
		}
	}
	else
	{
		if (!(read_from_standard()))
		{
			return false;
		}
	}

	return true;
}

int main(int argc, char* argv[])
//int main()
{
	std::vector<std::vector<int>> allLines;
	bool inputFile = false;
	bool outputFile = false;
	const char* inputFileName = "";
	const char* outputFileName = "";

	if (set_variables(argc, argv, inputFile, outputFile, inputFileName, outputFileName) != 0)
	{
		return 2;
	}

	std::streambuf* coutbuf{ std::cout.rdbuf() };
	if (outputFile)
	{
		std::ofstream outs{ outputFileName };
		if (!outs.is_open())
		{
			return 3;
		}
		std::cout.rdbuf(outs.rdbuf());

		if (!(sudoku_solver_caller(inputFile, inputFileName, allLines)))
		{
			return 2;
		}

		std::cout.rdbuf(coutbuf);
		outs.close();
	}
	else
	{
		if (!(sudoku_solver_caller(inputFile, inputFileName, allLines)))
		{
			return 2;
		}
	}

	return 0;
}
