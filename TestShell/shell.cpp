#include "shell.h"
#include<string>
#include<fstream>
#include<iostream>
#include <sstream>

Shell::Shell(ProcessExecutor* executor) : executor_(executor) {}

std::string Shell::read(int command) {
	executor_->readExecutor("aa.exe");

	std::ifstream ssdOutputFp("ssd_output.txt");
	std::string outputData = "";

	if (!ssdOutputFp.is_open()) {
		std::cout << "open fail\n";
	}
	if (!std::getline(ssdOutputFp, outputData)) return "";
	ssdOutputFp.close();

	std::string ret = findOutput(outputData, 0);
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