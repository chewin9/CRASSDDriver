#include "testscript.h"
#include <string>
#include <exception>
#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include "File.h"
#include "testscriptfactory.h"
#include "memory"

const int INVALID_INDEX = 0;

TestScriptRunner::TestScriptRunner(IProcessExecutor* exe, IFile* _file) : execute(exe), file(_file) {
	m_logger.disable_console_print();
}

std::shared_ptr<TestScript> TestScriptRunner::getScript(const std::string& commandLine) {
	return TestScriptFactory::getInstance().createTestScript(commandLine, m_logger);
}

bool TestScriptRunner::runScript(const std::string& commandLine) {
	m_logger.print(__func__, "Start runScript");
	std::shared_ptr<TestScript> script = TestScriptFactory::getInstance().createTestScript(commandLine, m_logger);

	if (script == nullptr) return false;

	return script->Run(execute, file);
}

bool TestScriptRunner::ScriptRunnerMode(std::string filename, IFile *file) {
	std::vector<std::string> scripts;

	m_logger.disable_console_print();

	scripts = file->ReadScriptFile(filename);

	for (auto cur_script : scripts) {
		if (runScript(cur_script) == false) {
			return false;
		}
	}
}

std::string TestScript::GetName() {
	return m_name;
}

std::string TestScript::makeWriteCommand(unsigned int addr, unsigned int value) {
	std::string format;
	char str[11];
	sprintf_s(str, "0x%08X", value);
	format = str;
	std::string result = GetSSDName() + " W " + std::to_string(addr) + " " + format;
	return result;
}

std::string TestScript::makeReadCommand(unsigned int addr) {
	std::string str = GetSSDName() + " R " + std::to_string(addr);
	return str;
}

std::string TestScript::makeEraseCommand(unsigned int addr, unsigned int size) {
	std::string result = GetSSDName() + " E " + std::to_string(addr) + " " + std::to_string(size);
	return result;
}

void TestScript::WriteBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned int value) {
	char buffer[50];  // maximum expected length of the float
	std::snprintf(buffer, 50, "Write at address : %d value : %08X", startaddr, value);
	std::string str(buffer);

	m_logger.print(__func__, str);
	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		exe->Process(makeWriteCommand(index, value));
	}
}

void TestScript::EraseBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len) {
	char buffer[50];  // maximum expected length of the float
	std::snprintf(buffer, 50, "Erase address : %d length : %d", startaddr, len);
	std::string str(buffer);

	m_logger.print(__func__, str);
	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		exe->Process(makeEraseCommand(index, len));
	}
}

void TestScript::PrintScriptEnter() {
	if (m_logger.is_diabled_console_print()) {
		std::cout << m_name << " ___ Run... ";
	}
}

void TestScript::PrintScriptExit(bool result) {
	if (m_logger.is_diabled_console_print()) {
		std::string res = (result == true) ? "Pass" : "Fail";
		std::cout << res << "\n";
	}
}

bool TestScript::ReadCompare(IProcessExecutor* exe, IFile* file, unsigned int startaddr, unsigned int len, unsigned value) {
	char buffer[100];  // maximum expected length of the float
	std::snprintf(buffer, 100, "%s : %d length : %d, value : %08X", __func__, startaddr, len, value);
	std::string str(buffer);

	m_logger.print(__func__, m_name + str);

	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		exe->Process(makeReadCommand(index));
		try {
			if (std::stoi(file->ReadOutputFile("ssd_output.txt").substr(2, 10), nullptr, 16) != value) {
				m_logger.print(__func__, "Read result is mismatched :" + m_name);
				return false;
			}
		}
		catch (std::exception e) {
			m_logger.print(__func__, "Thrown exception : stoi failed");
			return false;
		}
	}

	m_logger.print(__func__, "Read Success");
	return true;
}