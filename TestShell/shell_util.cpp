
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "shell_util.h"

void Util::printInvalidCommand() {
	std::cout << "INVALID COMMAND" << std::endl;
}

void Util::printInvalidParameter()
{
	std::cout << "INVALID PARAMETER" << std::endl;
}

std::vector<std::string> Util::splitBySpace(const std::string& cmd) {
	std::istringstream iss(cmd);
	std::vector<std::string> tokens;
	std::string word;
	while (iss >> word) {
		tokens.push_back(word);
	}
	return tokens;
}