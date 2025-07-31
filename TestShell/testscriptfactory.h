#pragma once
#include "testscript.h"

class DummyScript : public TestScript {
public:
	DummyScript(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe) override {
		//Script
		return false;
	}
};

class FullWriteAndReadCompare : public TestScript {
public:
	FullWriteAndReadCompare(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe) override;
};

class PartialLBAWrite : public TestScript {
public:
	PartialLBAWrite(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe) override;
	std::vector<std::string> value_list = { "0x00000001", "0x00000002","0x00000003","0x00000004","0x00000005" };

private:
	const unsigned int MAX_LOOP_COUNT = 30;
	const unsigned int MAX_TEST_AREA = 5;

	bool GetPartialReadAndCompareResult(IProcessExecutor* exe);
	void PartialBlockWrite(IProcessExecutor* exe);
};

class WriteReadAging : public TestScript {
public:
	WriteReadAging(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe) override;
};

class EraseAndWriteAging : public TestScript {
public:
	EraseAndWriteAging(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe) override;
};

class TestScriptFactory {
public:
	static TestScriptFactory& getInstance() {
		static TestScriptFactory instance;
		return instance;
	}
	static TestScript* createTestScript(std::string scriptname);
};