#pragma once
#include "testscript.h"

class FullWriteAndReadCompare : public TestScript {
public:
	FullWriteAndReadCompare(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe, IFile* file) override;
};

class PartialLBAWrite : public TestScript {
public:
	PartialLBAWrite(std::string name) : TestScript(name) {}
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
	WriteReadAging(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe, IFile* file) override;
};

class EraseAndWriteAging : public TestScript {
public:
	EraseAndWriteAging(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe, IFile* file) override;
};

class TestScriptFactory {
public:
	static TestScriptFactory& getInstance() {
		static TestScriptFactory instance;
		return instance;
	}

	std::shared_ptr<TestScript> createTestScript(const std::string& scriptname);

private:
	bool isMatch(std::string input, std::string scriptname);
};