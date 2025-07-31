#pragma once
#include <string>
#include <vector>

class File {
public:
	static std::string ReadOutputFile(const std::string& filename = "ssd_output.txt");
	static std::vector<std::string> ReadScriptFile(const std::string& filename = "scripts.txt");
};