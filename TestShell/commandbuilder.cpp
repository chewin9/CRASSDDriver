#include "commandbuilder.h"
#include <string>

std::string CommandBuilder::makeWriteCommand(unsigned int addr, unsigned int value) {
	std::string format;
	char str[11];
	sprintf_s(str, "0x%08X", value);
	format = str;
	std::string result = GetSSDName() + " W " + std::to_string(addr) + " " + format;
	return result;
}

std::string CommandBuilder::makeReadCommand(unsigned int addr) {
	std::string str = GetSSDName() + " R " + std::to_string(addr);
	return str;
}

std::string CommandBuilder::makeEraseCommand(unsigned int addr, unsigned int size) {
	std::string result = GetSSDName() + " E " + std::to_string(addr) + " " + std::to_string(size);
	return result;
}