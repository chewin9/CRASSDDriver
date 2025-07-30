/*
#include "FileIO.h"
#include <fstream>
#include <iostream>

static std::string ReadOutputFile(const std::string& filename) {
	std::string output;
	std::ifstream input(filename);
	if (!input.is_open()) {
		std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
		return output;
	}

	std::getline(input, output);
	input.close();
	return output;
}
*/