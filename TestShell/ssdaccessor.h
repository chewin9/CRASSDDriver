#pragma once
#include "iprocess_executor.h"
#include "File.h"
#include "commandbuilder.h"

class SsdAccessor {
public:
	void WriteBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned int value);
	void EraseBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len);
	bool ReadCompare(IProcessExecutor* exe, IFile* file, unsigned int startaddr, unsigned int len, unsigned value);
private:
	CommandBuilder commander;
};
