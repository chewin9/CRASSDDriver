#include "testscript.h"
#include <string>
#include <exception>
#include <iostream>
const int INVALID_INDEX = 0;

TestScriptRunner::TestScriptRunner(IProcessExecutor* exe) : execute(exe) {
	addScripts();
}

bool TestScriptRunner::runScript(const std::string& commandLine) {
	int commandIdx = 0;
	if ((commandIdx = parseCommandLine(commandLine)) == INVALID_INDEX) {
		return false;
	}

	return testScripts.at(commandIdx)->Run(execute);
}

void TestScriptRunner::addScripts() {
	testScripts.push_back(new DummyScript("0_Dummy"));
	testScripts.push_back(new FullWriteAndReadCompare("1_FullWriteAndReadCompare"));
	testScripts.push_back(new PartialLBAWrite("2_PartialLBAWrite"));
}

int TestScriptRunner::GetScriptIndex(const std::string& scriptname) {
	int scriptidx = 0;

	int seperator_position = scriptname.find('_');

	if ((seperator_position + 1) < scriptname.size()) return INVALID_INDEX;

	try {
		scriptidx = std::stoi(scriptname.substr(0, seperator_position));
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

std::string TestScript::GetName() {
	return m_name;
}

std::string TestScript::makeWriteCommand(IProcessExecutor* exe, unsigned int addr, unsigned int value) {
	std::string format;
	char str[11];
	sprintf_s(str, "0x%x", value);
	format = str;
	std::string result = "W " + std::to_string(addr) + " " + format;
	return result;
}

std::string TestScript::makeReadCommand(IProcessExecutor* exe, unsigned int addr) {
	std::string str = "R " + std::to_string(addr);
	return str;
}

void TestScript::WriteBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned int value) {
	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		exe->Process(makeWriteCommand(exe, index, value));
	}
}

bool TestScript::ReadCompare(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned value) {
	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		if (exe->Process(makeReadCommand(exe, index)) != value) {
			return false;
		}
	}

	return true;
}

bool PartialLBAWrite::Run(IProcessExecutor* exe)
{
	bool Read_0 = false;
	bool Read_1 = false;
	bool Read_2 = false;
	bool Read_3 = false;
	bool Read_4 = false;


	for (int i = 0; i < MAX_LOOP_COUNT; i++)
	{
		std::string R0 = "0xAAAAAAA0";
		std::string R1 = "0xAAAAAAA1";
		std::string R2 = "0xAAAAAAA2";
		std::string R3 = "0xAAAAAAA3";
		std::string R4 = "0xAAAAAAA4";

		exe->Process("ssd.exe W 0 " + R0);
		exe->Process("ssd.exe W 1 " + R1);
		exe->Process("ssd.exe W 2 " + R2);
		exe->Process("ssd.exe W 3 " + R3);
		exe->Process("ssd.exe W 4 " + R4);

		Read_0 = exe->Process("ssd.exe R 0") == 0xAAAAAAA0;
		Read_1 = exe->Process("ssd.exe R 1") == 0xAAAAAAA1;
		Read_2 = exe->Process("ssd.exe R 2") == 0xAAAAAAA2;
		Read_3 = exe->Process("ssd.exe R 3") == 0xAAAAAAA3;
		Read_4 = exe->Process("ssd.exe R 4") == 0xAAAAAAA4;

		if ((Read_0 && Read_1 && Read_2 && Read_3 && Read_4) == false) return false;
	}

	return true;
}