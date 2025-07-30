#pragma once
#include <string>

class File {
public:
	static std::string ReadOutputFile(const std::string& filename = "ssd_output.txt");
};