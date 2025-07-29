#pragma once
#include <string>
#include "iprocess_executor.h"

class TestScript {
public:
	TestScript(IProcessExecutor *exe) : execute(exe) {}
	void script1_FullWriteAndReadCompare();
	void script2_PartialLBAWrite();

private:
	IProcessExecutor* execute = nullptr;
};
