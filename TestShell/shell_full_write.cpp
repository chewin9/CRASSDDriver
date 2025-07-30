#pragma once
#include "iprocess_executor.h"
#include "shell_full_write.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

void ShellFullWrite::IssueFullWrite(std::string cmd) 
{
	std::vector<std::string> cmds = splitBySpace(cmd);
	std::string value = cmds[1];
	
	if (false == checkParameterValid(cmd)) {
		std::cout << "INVALID_COMMAND" << std::endl;
		return;
	}

	for (int nLBA = START_LBA; nLBA < END_LBA; nLBA++) {
		std::string cmdLine = "ssd.exe W " + std::to_string(nLBA) + " " + value;
		executor_->Process(cmdLine);
	}
	std::cout << "[WriteFull] Done" << std::endl;
}

bool ShellFullWrite::checkParameterValid(std::string& cmd)
{
	std::vector<std::string> cmds = splitBySpace(cmd);
	std::string value = cmds[1];
	if (cmds.size() != 2) {
		return false;
	}

	if (value.size() != 10) {
		return false;
	}

	if (value.find("0x") != 0) {
		return false;
	}

	if (false == is_valid_unsigned(value)) {
		return false;
	}
	return true;
}

std::vector<std::string> ShellFullWrite::splitBySpace(const std::string& str) {
	std::istringstream iss(str);
	std::vector<std::string> tokens;
	std::string word;
	while (iss >> word) {
		tokens.push_back(word);
	}
	return tokens;
}

bool ShellFullWrite::is_valid_unsigned(const std::string& str) {
	size_t idx = 0;
	try {
		std::stoul(str, &idx, 0); // base 0: auto-detect base (hex, dec, oct)
		return idx == str.size(); // true if entire string was used
	}
	catch (const std::invalid_argument&) {
		return false; // Not a number at all
	}
	catch (const std::out_of_range&) {
		return false; // Number outside unsigned long range
	}
}
