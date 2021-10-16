#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

/// <summary>
/// The function checks the length of the input, replaces the dots with zeros, checks whether the input 
/// consists only of numbers and writes the input from the string to the vector.
/// </summary>
/// <param name="strLine">Input line</param>
/// <param name="oneLine">Transformation of the input line into a vector of natural numbers</param>
/// <returns>True if the line is correct, otherwise false</returns>
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

/// <summary>
/// The function reads the lines from the input file, calls the function to check them and rewrite them into 
/// a vector, and adds the vector to the list of all vectors representing all the lines in the file. Function 
/// also checks for the correct opening and reading of all lines of the file.
/// </summary>
/// <param name="fileName">Input file name</param>
/// <param name="allLines">Vector of all lines in the file</param>
/// <returns>True if the whole file is read and checked, otherwise false</returns>
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

/// <summary>
/// The function checks whether the given number is in a row or in a column in sudoku.
/// </summary>
/// <param name="sudoku">A two-dimensional vector representing sudoku</param>
/// <param name="num">Searched number</param>
/// <param name="row">Number of the row in which the number is searched</param>
/// <param name="col">Number of the column in which the number is searched</param>
/// <returns>True if given number is in a row or in a column in sudoku, otherwise false</returns>
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

/// <summary>
/// A function that checks whether a given number is already in the square defined by the column and row number.
/// </summary>
/// <param name="sudoku">A two-dimensional vector representing sudoku</param>
/// <param name="num">Searched number</param>
/// <param name="row">The row number that is in the square, in which the number is searched</param>
/// <param name="col">The column number that is in the square, in which the number is searched</param>
/// <returns>True if given number is in a specified square in sudoku, otherwise false</returns>
bool is_in_square(const std::vector<std::vector<int>>& sudoku, int num, int row, int col)
{
	int squares = 3;

	// Finding the first row and column of the square in which the specified row and column are located
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

/// <summary>
/// Function that checks whether a given number can be inserted into sudoku.
/// </summary>
/// <param name="sudoku">A two-dimensional vector representing sudoku</param>
/// <param name="num">Searched number</param>
/// <param name="row">Number of the row in which the number is searched</param>
/// <param name="col">Number of the column in which the number is searched</param>
/// <returns>True if it is possible to insert a number in the selected place, otherwise false</returns>
bool is_fit(const std::vector<std::vector<int>>& sudoku, int num, int row, int col)
{
	return ((!is_in_row_or_col(sudoku, num, row, col)) && (!is_in_square(sudoku, num, row, col)));
}

/// <summary>
/// Function that checks if a given place in sudoku is empty.
/// </summary>
/// <param name="sudoku">A two-dimensional vector representing sudoku</param>
/// <param name="row">Row coordinate</param>
/// <param name="col">Column coordinate</param>
/// <returns>True if the given place in sudoku is empty, otherwise false</returns>
bool is_empty(const std::vector<std::vector<int>>& sudoku, int row, int col)
{
	return (sudoku[row][col] == 0);
}

/// <summary>
/// Check if each number is in each row exactly once.
/// </summary>
/// <param name="sudoku">A two-dimensional vector representing sudoku</param>
/// <returns>True if each number is in each row exactly once, otherwise false</returns>
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

/// <summary>
/// Check if each number is in each column exactly once.
/// </summary>
/// <param name="sudoku">A two-dimensional vector representing sudoku</param>
/// <returns>True if each number is in each column exactly once, otherwise false</returns>
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

/// <summary>
/// Check if each number is in each square exactly once.
/// </summary>
/// <param name="sudoku">A two-dimensional vector representing sudoku</param>
/// <returns>True if each number is in each square exactly once, otherwise false</returns>
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

/// <summary>
/// Check if each number is in each row, column and square exactly once.
/// </summary>
/// <param name="sudoku">A two-dimensional vector representing sudoku</param>
/// <returns>True if each number is in each row, column and square exactly once, otherwise false</returns>
bool check_sudoku(const std::vector<std::vector<int>>& sudoku)
{
	return (check_rows(sudoku) && check_cols(sudoku) && check_squares(sudoku));
}

/// <summary>
/// Recursive function to solve sudoku.
/// </summary>
/// <param name="sudoku">A two-dimensional vector representing sudoku</param>
/// <returns>True, if sudoku is solved, otherwise false</returns>
bool solve_sudoku_recursive(std::vector<std::vector<int>>& sudoku)
{	
	int row = 0;
	int col = 0;
	bool empty = false;

	// Find first free place in sudoku
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

	// If no free place found, return true
	if (empty)
	{
		// Iterate through the numbers 1 to 9
		for (int num = 1; num <= 9; num++)
		{
			// If the number fits, insert it
			if (is_fit(sudoku, num, row, col))
			{
				sudoku[row][col] = num;

				// Recursive call
				if (solve_sudoku_recursive(sudoku))
				{
					return true;
				}

				// If the number was placed incorrectly, delete it
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

/// <summary>
/// The function transforms the sudoku specified in the line into a two-dimensional vector representing sudoku. 
/// The function then calls a function to recursively solve sudoku and a function to verify that sudoku has been 
/// solved correctly. If sudoku has been solved correctly, it will print it. If sudoku was not solved correctly, 
/// it will print a blank line.
/// </summary>
/// <param name="oneLineFull">Vector representing one sudoku</param>
/// <returns>0</returns>
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

	if (solve_sudoku_recursive(sudoku) && check_sudoku(sudoku))
	{		
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

/// <summary>
/// The function sets variables based on the input parameters.
/// </summary>
/// <param name="argc">Argument count</param>
/// <param name="argv">Argument vector</param>
/// <param name="inputFile">Boolean whether sudoku will be loaded from a file or not</param>
/// <param name="outputFile">Boolean whether sudoku will be written to a file or not</param>
/// <param name="inputFileName">Input file name</param>
/// <param name="outputFileName">Output file name</param>
/// <returns>0 if the variables were set correctly, 2 if not</returns>
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

/// <summary>
/// The function reads the lines from the input, checks them, and if they are entered correctly, solves them.
/// </summary>
/// <returns>True if all sudoku puzzles were entered correctly and solved, otherwise false</returns>
bool read_and_solve_from_standard()
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

/// <summary>
/// Based on the input parameters, the function selects whether to read from a file or from standard input and solves sudoku.
/// </summary>
/// <param name="inputFile">Boolean whether sudoku will be loaded from a file or not</param>
/// <param name="inputFileName">Input file name</param>
/// <param name="allLines">Vector of all lines in the file</param>
/// <returns>True if all sudoku puzzles were loaded correctly and solved, otherwise false</returns>
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
		if (!(read_and_solve_from_standard()))
		{
			return false;
		}
	}

	return true;
}

/// <summary>
/// The main function of the program. The function sets the values of the variables, selects the output method, and calls functions to solve sudoku.
/// </summary>
/// <param name="argc">Argument count</param>
/// <param name="argv">Argument vector</param>
/// <returns>0 if the program worked properly, 2 if there was a problem with variables or sudoku, 3 if the output file could not be opened</returns>
int main(int argc, char* argv[])
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
