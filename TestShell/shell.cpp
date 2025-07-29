#include "shell.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

Shell::Shell(ProcessExecutor* executor) : executor_(executor) {}

std::string Shell::read(int index) {
	executor_->Process("ssd_executor.exe");

	std::ifstream ssdOutputFp("ssd_output.txt");

	if (!ssdOutputFp.is_open()) {
		std::cout << "open fail\n";
	}
	std::string outputData((std::istreambuf_iterator<char>(ssdOutputFp)),
		std::istreambuf_iterator<char>());
	ssdOutputFp.close();

	std::string ret = findOutput(outputData, index);
	return ret;
}

std::string Shell::findOutput(const std::string &data, int index) {
	std::istringstream iss(data);
	std::string line;
	while (std::getline(iss, line)) {
		std::istringstream line_stream(line);
		int idx;
		std::string value;
		if (line_stream >> idx >> value) {
			if (idx == index) {
				return value;
			}
		}
	}
}