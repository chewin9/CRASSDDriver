#include "testscript.h"
#include <iostream>

TestScriptRunner::TestScriptRunner(IProcessExecutor* exe) : execute(exe) {
	addScripts();
}

void TestScriptRunner::runScript(const std::string& commandLine) {
	int commandIdx = 0;
	if ((commandIdx = parseCommandLine(commandLine)) == 0) {
		return;
	}

	testScripts.at(commandIdx)->Run();
}

void TestScriptRunner::addScripts() {
	testScripts.push_back(new FullWriteAndReadCompare("1_FullWriteAndReadCompare"));
}

int TestScriptRunner::parseCommandLine(const std::string& commandLine) {

	return 1;
}

void TestScript::script2_PartialLBAWrite(void)
{
	
}