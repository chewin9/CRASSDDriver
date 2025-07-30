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

bool TestScript::script2_PartialLBAWrite(void)
{
	bool Read_0 = false;
	bool Read_1 = false;
	bool Read_2 = false;
	bool Read_3 = false;
	bool Read_4 = false;


	for (int i = 0; i < 30; i++)
	{
		std::string R0 = "0xAAAAAAA0";
		std::string R1 = "0xAAAAAAA1";
		std::string R2 = "0xAAAAAAA2";
		std::string R3 = "0xAAAAAAA3";
		std::string R4 = "0xAAAAAAA4";

		execute->Process("ssd.exe W 0 " + R0);
		execute->Process("ssd.exe W 1 " + R1);
		execute->Process("ssd.exe W 2 " + R2);
		execute->Process("ssd.exe W 3 " + R3);
		execute->Process("ssd.exe W 4 " + R4);

		Read_0 = execute->Process("ssd.exe R 0") == 0xAAAAAAA0;
		Read_1 = execute->Process("ssd.exe R 1") == 0xAAAAAAA1;
		Read_2 = execute->Process("ssd.exe R 2") == 0xAAAAAAA2;
		Read_3 = execute->Process("ssd.exe R 3") == 0xAAAAAAA3;
		Read_4 = execute->Process("ssd.exe R 4") == 0xAAAAAAA4;

		if ((Read_0 && Read_1 && Read_2 && Read_3 && Read_4) == false) return false;
	}

	return true;
}