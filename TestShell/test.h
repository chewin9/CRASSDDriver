#pragma once
#include "File.h"
#include "iprocess_executor.h"
class MockProcessExecutor : public IProcessExecutor {
public:
	MOCK_METHOD(int, Process, (const std::string& commandLine), (override));
};

class MockFile : public IFile {
public:
	MOCK_METHOD(std::string, ReadOutputFile, (const std::string& filename), (override));
	MOCK_METHOD(std::vector<std::string>, ReadScriptFile, (const std::string& filename), (override));
};