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
	std::cout << "Checking for prices file... " << std::endl;

	const char* fileName = "prices.txt";
	out.clear();
	ReadTxtFile(fileName, 
		[&](std::string& line, uint32_t lineNumber) { out.push_back(line); }
	);
}
