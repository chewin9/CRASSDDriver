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
#include "ssdaccessor.h"

const int MAX_ADDR = 99;

class TestScript {
public:

	TestScript(std::string name) : m_name(name) {

	}

	virtual bool Run(IProcessExecutor* exe, IFile* file) = 0;
	std::string GetName();

protected:
	std::string m_name = nullptr;
	SsdAccessor accessor;
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
