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
	std::string makeEraseCommand(unsigned int addr, unsigned int size);
	void WriteBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned int value);
	bool ReadCompare(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned value);
	void EraseBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len);
	std::string GetSSDName() { return SSD_NAME; }
	std::string ReadOutputFile(const std::string& filename = "ssd_output.txt");

protected:
	std::string m_name = nullptr;
	const std::string SSD_NAME = "SSDDriver.exe";
};

class TestScriptRunner {
public:
	TestScriptRunner(IProcessExecutor* exe);
	bool runScript(const std::string& commandLine);
	bool IsValidSciprtCommand(const std::string& commandLine);
	bool ScriptRunnerMode(std::string filename);

private:
	//Method
	IProcessExecutor* execute = nullptr;
	int parseCommandLine(const std::string& commandLine);
	int GetScriptIndex(const std::string& scriptname);
	void addScripts();

	//Variable
	std::vector<TestScript*> testScripts;
};
