#include "ssdaccessor.h"
#include "testscriptlog.h"
#include "commandbuilder.h"
#include "Logger.h"

void SsdAccessor::WriteBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned int value) {
	char buffer[50];  // maximum expected length of the float
	std::snprintf(buffer, 50, "Write at address : %d value : %08X", startaddr, value);
	std::string str(buffer);

	PRINT_NO_NAME(str);
	for (unsigned int cur_addr = startaddr; cur_addr < startaddr + len; cur_addr++) {
		exe->Process(commander.makeWriteCommand(cur_addr, value));
	}
}

void SsdAccessor::EraseBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len) {
	char buffer[50];  // maximum expected length of the float
	std::snprintf(buffer, 50, "Erase address : %d length : %d", startaddr, len);
	std::string str(buffer);

	PRINT_NO_NAME(str);
	for (unsigned int cur_addr = startaddr; cur_addr < startaddr + len; cur_addr++) {
		exe->Process(commander.makeEraseCommand(cur_addr, len));
	}
}

bool SsdAccessor::ReadCompare(IProcessExecutor* exe, IFile* file, unsigned int startaddr, unsigned int len, unsigned int value) {
	char buffer[100];
	const int outStartPos = 2;
	const int outEndPos = 10;
	const int HEX = 16;
	std::snprintf(buffer, 100, "%s : %d length : %d, value : %08X", __func__, startaddr, len, value);
	std::string str(buffer);

	PRINT_NO_NAME(str);

	for (unsigned int cur_addr = startaddr; cur_addr < startaddr + len; cur_addr++) {
		exe->Process(commander.makeReadCommand(cur_addr));
		try {
			if (std::stoul(file->ReadOutputFile("ssd_output.txt").substr(outStartPos, outEndPos), nullptr, HEX) != value) {
				PRINT_NO_NAME("Read result is mismatched :");
				return false;
			}
		}
		catch (std::exception e) {
			PRINT_NO_NAME("Thrown exception : stoi failed");
			return false;
		}
	}

	PRINT_NO_NAME("Read Success");
	return true;
}