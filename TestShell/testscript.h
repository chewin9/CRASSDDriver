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
	std::string makeWriteCommand(IProcessExecutor* exe, unsigned int addr, unsigned int value);
	std::string makeReadCommand(IProcessExecutor* exe, unsigned int addr);
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
	bool Run(IProcessExecutor* exe) override {
		//Script
		int value = 5;
		int start = 0;
		const int length = 5;

		for (start = 0; start < MAX_ADDR; start += length) {
			for (int index = start;index < start + length; index++) {
				WriteBlock(exe, start, 5, value);
				if (ReadCompare(exe, start, length, value) == false) return false;
			}
		}

		return true;
	}
};

class PartialLBAWrite : public TestScript {
public:
	PartialLBAWrite(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe) override;
private:
	const unsigned int MAX_LOOP_COUNT = 30;
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
