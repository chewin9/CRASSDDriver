#include "File.h"
#include <fstream>
#include <iostream>

std::string File::ReadOutputFile(const std::string& filename) {
	std::string output;
	std::ifstream input(filename);
	if (!input.is_open()) {
		std::cerr << "������ �� �� �����ϴ�: " << filename << std::endl;
		return output;
	}

	std::getline(input, output);
	input.close();
	return output;
}

std::vector<std::string> File::ReadScriptFile(const std::string& filename) {
	std::vector<std::string> output_arr;
	std::string output;
	std::ifstream input(filename);
	if (!input.is_open()) {
		std::cerr << "������ �� �� �����ϴ�: " << filename << std::endl;
		return output_arr;
	}

	while (std::getline(input, output)) {
		output_arr.push_back(output);
	}

	input.close();
	return output_arr;
}