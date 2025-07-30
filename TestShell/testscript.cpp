#include "testscript.h"
#include <string>
#include <exception>

const int INVALID_INDEX = 0;

TestScriptRunner::TestScriptRunner(IProcessExecutor* exe) : execute(exe) {
	addScripts();
}

bool TestScriptRunner::runScript(const std::string& commandLine) {
	int commandIdx = 0;
	if ((commandIdx = parseCommandLine(commandLine)) == 0) {
		return false;
	}

	testScripts.at(commandIdx)->Run(execute);
}

void TestScriptRunner::addScripts() {
	testScripts.push_back(new DummyScript("0_Dummy"));
	testScripts.push_back(new FullWriteAndReadCompare("1_FullWriteAndReadCompare"));
}

int TestScriptRunner::GetScriptIndex(const std::string& scriptname) {
	int scriptidx = 0;

	try {
		scriptidx = std::stoi(scriptname);
	}
	catch (std::exception e) {
		return INVALID_INDEX;
	}

	return scriptidx;
}

int TestScriptRunner::parseCommandLine(const std::string& commandLine) {
	int scriptidx = INVALID_INDEX;
	if ((scriptidx = GetScriptIndex(commandLine)) != INVALID_INDEX && scriptidx > 0 && scriptidx < testScripts.size()) {
		return scriptidx;
	}

	for (int index = 0; index < testScripts.size(); index++) {
		if (commandLine == testScripts[index]->GetName()) {
			return index;
		}
	}
	return INVALID_INDEX;
}
