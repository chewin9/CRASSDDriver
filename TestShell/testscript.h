#pragma once
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include "iprocess_executor.h"
#include "File.h"

const int MAX_ADDR = 99;

class TestScript {
public:

	TestScript(std::string name) : m_name(name) {

	}

	virtual bool Run(IProcessExecutor* exe, IFile* file) = 0;
	std::string GetName();
	std::string makeWriteCommand(unsigned int addr, unsigned int value);
	std::string makeReadCommand(unsigned int addr);
	std::string makeEraseCommand(unsigned int addr, unsigned int size);
	void WriteBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned int value);
	void EraseBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len);
	bool ReadCompare(IProcessExecutor* exe, IFile* file, unsigned int startaddr, unsigned int len, unsigned value);
	std::string GetSSDName() { return SSD_NAME; }

protected:
	std::string m_name = nullptr;
	const std::string SSD_NAME = "SSDDriver.exe";
};

class TestScriptRunner {
public:
	TestScriptRunner(IProcessExecutor* exe, IFile* _file);
	bool runScript(const std::string& commandLine);
	bool IsValidSciprtCommand(const std::string& commandLine);

private:
	//Method
	IProcessExecutor* execute = nullptr;
	IFile* file = nullptr;
	int parseCommandLine(const std::string& commandLine);
	int GetScriptIndex(const std::string& scriptname);
	void addScripts();

	//Variable
	std::vector<TestScript*> testScripts;
};
