#pragma once
#include <string>
#include <vector>
#include "iprocess_executor.h"

class TestScript {
public:
	TestScript(std::string name) : m_name(name) {

	}

	virtual void Run() = 0;
	std::string GetName() {
		return m_name;
	}

protected:
	std::string m_name = nullptr;
};

class FullWriteAndReadCompare : public TestScript {
public:
	FullWriteAndReadCompare(std::string name) : TestScript(name) {}
	void Run() override {
		//Script
	}
};

class PartialLBAWrite : public TestScript {
public:
	PartialLBAWrite(std::string name) : TestScript(name) {}
	void Run(void) override {

	}
};

class TestScriptRunner {
public:
	TestScriptRunner(IProcessExecutor* exe);
	void runScript(const std::string& commandLine);

private:
	//Method
	IProcessExecutor* execute = nullptr;
	int parseCommandLine(const std::string& commandLine);
	void addScripts();

	//Variable
	std::vector<TestScript*> testScripts;
};
