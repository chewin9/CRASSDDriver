#pragma once
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include "iprocess_executor.h"

const int MAX_ADDR = 99;

class TestScript {
public:

	TestScript(std::string name) : m_name(name) {

	}

	virtual bool Run(IProcessExecutor* exe) = 0;
	std::string GetName();
	std::string makeWriteCommand(unsigned int addr, unsigned int value);
	std::string makeReadCommand(unsigned int addr);
	void WriteBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned int value);
	bool ReadCompare(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned value);

protected:
	std::string m_name = nullptr;
};

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
private:
	const unsigned int MAX_LOOP_COUNT = 30;
};

class WriteReadAging : public TestScript {
public:
	WriteReadAging(std::string name) : TestScript(name) {}
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
	int GetScriptIndex(const std::string& scriptname);
	void addScripts();

	//Variable
	std::vector<TestScript*> testScripts;
};
