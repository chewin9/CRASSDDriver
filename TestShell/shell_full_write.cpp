#pragma once
#include "iprocess_executor.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "shell_full_write.h"
void ShellFullWrite::IssueFullWrite(std::string cmd) 
{
	std::vector<std::string> cmds = splitBySpace(cmd);

	std::string value = cmds[1];

	for (int nLBA = 0; nLBA < 100; nLBA++) {
		std::string cmdLine = "ssd.exe W " + std::to_string(nLBA) + " " + value;
		executor_->Process(cmdLine);
	}
	std::cout << "[WriteFull] Done" << std::endl;
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
