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

class DummyScript : public TestScript {
public:
	DummyScript(std::string name) : TestScript(name) {}
	bool Run(IProcessExecutor* exe, IFile* file) override {
		//Script
		return false;
	}
};

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
