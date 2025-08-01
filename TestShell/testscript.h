#pragma once
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <memory>
#include "iprocess_executor.h"
#include "File.h"
#include "logger.h"

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
	Logger* m_plogger;
	const std::string SSD_NAME = "SSDDriver.exe";
	void PrintScriptEnter();
	void PrintScriptExit(bool result);
};

class TestScriptRunner {
public:
	TestScriptRunner(IProcessExecutor* exe, IFile* _file);
	bool runScript(const std::string& commandLine);
	std::shared_ptr<TestScript> getScript(const std::string& commandLine);
	bool ScriptRunnerMode(std::string filename, IFile* _file);

private:
	//Method
	IProcessExecutor* execute = nullptr;
	IFile* file = nullptr;
	//Variable
	std::vector<TestScript*> testScripts;
};
