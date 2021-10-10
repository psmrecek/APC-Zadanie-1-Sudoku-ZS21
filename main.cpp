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
		std::cout << "Invalid file" << std::endl;
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
		//std::cout << "File read successful" << std::endl;
		return true;
	}

	return false;
}

bool read_from_standard(std::vector<std::vector<int>>& allLines)
{
	std::string strLine;
	std::getline(std::cin, strLine);

	std::vector<int> oneLine;

	if (!(check_valid_input_line(strLine, oneLine)))
	{
		return false;
	}

	allLines.push_back(oneLine);

	return true;
}

bool isInRowOrCol(const std::vector<std::vector<int>>& sudoku, int num, int row, int col)
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

bool isInSquare(const std::vector<std::vector<int>>& sudoku, int num, int row, int col)
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

bool isFit(const std::vector<std::vector<int>>& sudoku, int num, int row, int col)
{
	return ((!isInRowOrCol(sudoku, num, row, col)) && (!isInSquare(sudoku, num, row, col)));
}

bool isEmpty(const std::vector<std::vector<int>>& sudoku, int row, int col)
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

		if (isEmpty(sudoku, row, col))
		{
			empty = true;
			break;
		}
	}

	if (empty)
	{
		for (int num = 1; num <= 9; num++)
		{
			if (isFit(sudoku, num, row, col))
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

int main(int argc, char* argv[])
//int main()
{
	std::vector<std::vector<int>> allLines;
	bool inputFile = false;
	bool outputFile = false;
	auto inputFileName = "";
	auto outputFileName = "";

	//std::cout << argc << std::endl;
	//for (int i = 0; i < argc; i++)
	//{
	//	std::cout << argv[i] << " ";
	//}
	//std::cout << std::endl;

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

	if (inputFile)
	{
		if (!(read_from_file(inputFileName, allLines)))
		{
			return 2;
		}
	}
	else
	{
		if (!(read_from_standard(allLines)))
		{
			return 2;
		}
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

		for (size_t i = 0; i < allLines.size(); i++)
		{
			sudoku_solver(allLines[i]);
		}
		std::cout.rdbuf(coutbuf);
		outs.close();
	}
	else
	{
		for (size_t i = 0; i < allLines.size(); i++)
		{
			sudoku_solver(allLines[i]);
		}
	}


	return 0;
}


//	if (std::string(argv[1]) != "-i")
//	{
//		std::cerr << "Invalid flag";
//		return 1;
//	}


//	std::ifstream ifs{ "input.txt" };
//	// Ma viacero stavov, treba si na to davat pozor
//
//	std::string line;
//
//	if (!ifs.is_open())
//	{
//		std::cout << "Invalid file" << std::endl;
//		return 1;
//	}
//
//	while (std::getline(ifs, line))
//	{
//		std::cout << line << std::endl;
//		// Neriesit konce riadkov v subore vobec, nechat to na standardnu kniznicu
//		// Ako koniec pouzivat idealne std::endl, ale da sa aj klasicke "\n"
//
//	}
//
//	if (ifs.eof())
//	{
//		std::cout << "File read successful" << std::endl;
//		return 0;
//	}





//std::vector<int> vec1;
//std::vector<int> vec2;
//std::vector<int> vec3;
//std::vector<std::vector<int>> vecvec;
//
//for (int i = 0; i < 10; i++)
//{
//	vec1.push_back(i + 10);
//	vec2.push_back(i + 100);
//	vec3.push_back(i + 1000);
//}
//
//for (int i = 0; i < 10; i++)
//{
//	vec2.push_back(i + 1000);
//	vec3.push_back(i + 10000);
//}
//
//for (int i = 0; i < 10; i++)
//{
//	vec3.push_back(i + 100000);
//}
//
//vec1.push_back('a');
//
//vecvec.push_back(vec1);
//vecvec.push_back(vec2);
//vecvec.push_back(vec3);
//
//for (size_t i = 0; i < vecvec.size(); i++)
//{
//	std::cout << std::endl << vecvec[i].size() << ": ";
//	for (size_t j = 0; j < vecvec[i].size(); j++)
//	{
//		std::cout << vecvec[i][j] << " ";
//	}
//}