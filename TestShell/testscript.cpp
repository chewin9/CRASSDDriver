#include "testscript.h"
#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include "File.h"
#include "testscriptfactory.h"
#include "memory"

#include "testscriptlog.h"

const int INVALID_INDEX = 0;

TestScriptRunner::TestScriptRunner(IProcessExecutor* exe, IFile* _file) : execute(exe), file(_file) {
#if defined (_DEBUG)
	Logger::getInstance().disable_console_print();
#endif
}

std::shared_ptr<TestScript> TestScriptRunner::getScript(const std::string& commandLine) {
	return TestScriptFactory::getInstance().createTestScript(commandLine);
}

bool TestScriptRunner::runScript(const std::string& commandLine) {
	PRINT_NO_NAME("Start runScript");
	std::shared_ptr<TestScript> script = TestScriptFactory::getInstance().createTestScript(commandLine);

	if (script == nullptr) return false;

	return script->Run(execute, file);
}

bool TestScriptRunner::ScriptRunnerMode(std::string filename, IFile *file) {
	std::vector<std::string> scripts;

	Logger::getInstance().disable_console_print();

	scripts = file->ReadScriptFile(filename);

	for (auto cur_script : scripts) {
		if (runScript(cur_script) == false) {
			return false;
		}
	}
}

void TestScript::PrintScriptEnter() {
	if (Logger::getInstance().is_diabled_console_print()) {
		std::cout << m_name << " ___ Run... ";
	}
}

void TestScript::PrintScriptExit(bool result) {
	if (Logger::getInstance().is_diabled_console_print()) {
		std::string res = (result == true) ? "Pass" : "Fail";
		std::cout << res << std::endl;
	}
}
