#include "shell_read.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
ShellRead::ShellRead(IProcessExecutor* executor) : executor_(executor) {}

bool ShellRead::Run(const std::string& input) {
	std::vector<std::string> commandVector = splitBySpace(input);

	if (checkParameterValid(commandVector) == false) {
		printInvalidCommand();
		return false;
	}

	//SSD 수행
	performReadToSSD(commandVector.at(LBA_INDEX));
	
	//결과 값 읽기
	std::string SsdData = getSsdOutputData();
	int index = std::stoi(commandVector.at(LBA_INDEX));
	printResult(index, SsdData);

	return true;
}

std::string Read::getSsdOutputData(void) {

	std::ifstream file(SSD_OUTPUT_FILE);
	if (!file.is_open()) {
		std::cout << "open fail\n";
		return "";
	}
	std::string ret;
	std::getline(file, ret);
	file.close();

	return ret;
}

void Read::printInvalidCommand() {
	std::cout << INVALID_COMMAND << "\n";
}

std::vector<std::string> Read::splitBySpace(const std::string& cmd) {
	std::istringstream iss(cmd);
	std::vector<std::string> tokens;
	std::string word;
	while (iss >> word) {
		tokens.push_back(word);
	}
	return tokens;
}

void ShellRead::printResult(int index, std::string value) {
	if (value == ERROR_RETURN) {
		std::cout << ERROR_RETURN << "\n";
	}
	else {
		std::cout << "[Read] LBA ";
		std::cout << std::setw(2) << std::setfill('0') << index << " ";
		std::cout << value << "\n";
	}
}

bool ShellRead::checkParameterValid(std::vector<std::string> commandVec) {
	if (commandVec.size() != VALID_READ_COMMAND_SIZE) {
		return false;
	}

	if (isDecimalString(commandVec.at(LBA_INDEX)) == false) {
		return false;
	}

	int index;
	try {
		index = std::stoi(commandVec.at(LBA_INDEX));
	}catch (const std::invalid_argument&) {
		return false; // 숫자가 아님
	}
	catch (const std::out_of_range&) {
		return false; // 범위를 벗어남
	}
	return true;
}

void ShellRead::performReadToSSD(std::string index) {
	std::string cmdLine = "SSDDriver.exe R " + index;
	executor_->Process(cmdLine);
}

bool Read::isDecimalString(const std::string& str) {
	if (str.empty()) return false;
	size_t i = 0;
	if (str[0] == '-' || str[0] == '+') i = 1;
	if (i == str.size()) return false; // 부호만 있는 경우
	for (; i < str.size(); ++i) {
		if (!std::isdigit(str[i])) return false;
	}
	return true;
}