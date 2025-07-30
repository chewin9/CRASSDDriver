#pragma once
#include <string>
#include <vector>
#include "iprocess_executor.h"

class TestScript {
public:

	TestScript(std::string name) : m_name(name) {

	}

	virtual bool Run(IProcessExecutor* exe) = 0;
	std::string GetName() {
		return m_name;
	}

protected:
	std::string m_name = nullptr;
};

class FullWriteAndReadCompare : public TestScript {
public:
	FullWriteAndReadCompare(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe) override {
		//Script
		return false;
	}
};

class PartialLBAWrite : public TestScript {
public:
	PartialLBAWrite(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe) override;
};

class TestScriptRunner {
public:
	TestScriptRunner(IProcessExecutor* exe);
	bool runScript(const std::string& commandLine);

private:
	//Method
	IProcessExecutor* execute = nullptr;
	int parseCommandLine(const std::string& commandLine);
	void addScripts();

	//Variable
	std::vector<TestScript*> testScripts;
};
