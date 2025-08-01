#pragma once
#include "testscript.h"

class DummyScript : public TestScript {
public:
	DummyScript(std::string name, Logger& logger) : TestScript(name, logger) {}
	bool Run(IProcessExecutor* exe, IFile* file) override {
		//Script
		return false;
	}
};

class FullWriteAndReadCompare : public TestScript {
public:
	FullWriteAndReadCompare(std::string name, Logger& logger) : TestScript(name, logger) {}
	bool Run(IProcessExecutor* exe, IFile* file) override;
};

class PartialLBAWrite : public TestScript {
public:
	PartialLBAWrite(std::string name, Logger& logger) : TestScript(name, logger) {}
	bool Run(IProcessExecutor* exe, IFile* file) override;
	std::vector<std::string> value_list = { "0x00000001", "0x00000002","0x00000003","0x00000004","0x00000005" };

private:
	const unsigned int MAX_LOOP_COUNT = 30;
	const unsigned int MAX_TEST_AREA = 5;

	bool GetPartialReadAndCompareResult(IProcessExecutor* exe, IFile* file);
	void PartialBlockWrite(IProcessExecutor* exe);
};

class WriteReadAging : public TestScript {
public:
	WriteReadAging(std::string name, Logger& logger) : TestScript(name, logger) {}
	bool Run(IProcessExecutor* exe, IFile* file) override;
};

class EraseAndWriteAging : public TestScript {
public:
	EraseAndWriteAging(std::string name, Logger& logger) : TestScript(name, logger) {}
	bool Run(IProcessExecutor* exe, IFile* file) override;
};

class TestScriptFactory {
public:
	static TestScriptFactory& getInstance() {
		static TestScriptFactory instance;
		return instance;
	}

	std::shared_ptr<TestScript> createTestScript(const std::string& scriptname, Logger& logger);

private:
	bool isMatch(std::string input, std::string scriptname);
};