#pragma once
#include <string>
#include "iprocess_executor.h"

class TestScript {
public:
	TestScript(IProcessExecutor *exe) : execute(exe) {}
	void script1_FullWriteAndReadCompare();

private:
	IProcessExecutor* execute = nullptr;
};
