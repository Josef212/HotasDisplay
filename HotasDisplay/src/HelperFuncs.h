#pragma once

#include <vector>
#include <string>
#include <functional>

typedef std::function<void(std::string& line, uint32_t lineNumber)> OnLineReadFn;

bool ReadTxtFile(const char* path, OnLineReadFn lineCbk = nullptr);
void GetPainitePrice(std::vector<std::string>& out);
void ReadPainitePrice(std::vector<std::string>& out);
