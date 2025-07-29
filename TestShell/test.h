#pragma once

class MockProcessExecutor : public IProcessExecutor {
public:
	MOCK_METHOD(int, Process, (const std::string& commandLine), (override));
};