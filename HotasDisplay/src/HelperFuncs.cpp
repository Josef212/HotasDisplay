#include "stdafx.h"
#include "HelperFuncs.h"

bool ReadTxtFile(const char* path, OnLineReadFn lineCbk)
{
	std::cout << "Checking txt file: " << path << " - ";

	struct stat buffer;
	if (stat(path, &buffer) == 0)
	{
		std::cout << "FOUND. \nReading... ";

		std::ifstream stream(path);
		std::string line;
		int lineNumber = 0;
		size_t strSize;

		if (stream.is_open() && lineCbk != nullptr)
		{
			while (std::getline(stream, line))
				lineCbk(line, lineNumber++);
		}

		stream.close();
		std::cout << "File closed." << std::endl;

		return true;
	}

	return false;
}

void GetPainitePrice(std::vector<std::string>& out)
{
	system("node scripts/price_request.js");
	ReadPainitePrice(out);
}

void ReadPainitePrice(std::vector<std::string>& out)
{
	std::cout << "Checking for prices file... ";
	const char* fileName = "prices.txt";

	struct stat buffer;
	if (stat(fileName, &buffer) == 0)
	{
		std::cout << "FOUND. \Reading price." << std::endl;
		std::ifstream stream(fileName);
		std::string line;
		int lineNumber = 0;
		size_t strSize;

		if (stream.is_open())
		{
			out.clear();

			while (std::getline(stream, line))
			{
				std::cout << line << std::endl;
				if (lineNumber >= 0 && lineNumber < 3)
				{
					out.push_back(line);
				}

				++lineNumber;
			}

			stream.close();
			std::cout << std::endl;
		}
	}
}
