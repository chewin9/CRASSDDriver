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
	std::string GetName() {
		return m_name;
	}

	std::string makeWriteCommand(IProcessExecutor* exe, unsigned int addr, unsigned int value) {
		std::string format;
		char str[11];
		sprintf_s(str, "0x%x", value);
		format = str;
		std::string result = "W " + std::to_string(addr) + " " + format;
		return result;
	}

	std::string makeReadCommand(IProcessExecutor* exe, unsigned int addr) {
		std::string str = "R " + std::to_string(addr);
		return str;
	}

	void WriteBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned int value) {
		for (unsigned int index = startaddr; index < startaddr + len; index++) {
			exe->Process(makeWriteCommand(exe, index, value));
		}
	}

	bool ReadCompare(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned value) {
		for (unsigned int index = startaddr; index < startaddr + len; index++) {
			if (exe->Process(makeReadCommand(exe, index)) != value) {
				return false;
			}
		}

		return true;
	}

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
