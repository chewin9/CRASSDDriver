#pragma once
class Util {
public:
	static void printInvalidCommand();
	static void printInvalidParameter();

	static std::vector<std::string> splitBySpace(const std::string& cmd);
};
