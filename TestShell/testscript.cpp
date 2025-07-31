#include "testscript.h"
#include <string>
#include <exception>
#include <iostream>
#include <random>
#include <ctime>
#include "File.h"
#include "testscriptfactory.h"

const int INVALID_INDEX = 0;

TestScriptRunner::TestScriptRunner(IProcessExecutor* exe, IFile* _file) : execute(exe), file(_file) {
	addScripts();
}

bool TestScriptRunner::runScript(const std::string& commandLine) {
	int commandIdx = 0;
	if ((commandIdx = parseCommandLine(commandLine)) == INVALID_INDEX) {
		return false;
	}

	return testScripts.at(commandIdx)->Run(execute, file);
}

bool TestScriptRunner::IsValidSciprtCommand(const std::string& commandLine) {
	if (parseCommandLine(commandLine) == INVALID_INDEX) {
		return false;
	}

	return true;
}

void TestScriptRunner::addScripts() {
	testScripts.push_back(TestScriptFactory::getInstance().createTestScript("0_Dummy"));
	testScripts.push_back(TestScriptFactory::getInstance().createTestScript("1_FullWriteAndReadCompare"));
	testScripts.push_back(TestScriptFactory::getInstance().createTestScript("2_PartialLBAWrite"));
	testScripts.push_back(TestScriptFactory::getInstance().createTestScript("3_WriteReadAging"));
	testScripts.push_back(TestScriptFactory::getInstance().createTestScript("4_EraseAndWriteAging"));
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
	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		exe->Process(makeWriteCommand(index, value));
	}
}

void TestScript::EraseBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len) {
	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		exe->Process(makeEraseCommand(index, len));
	}
}
bool TestScript::ReadCompare(IProcessExecutor* exe, IFile* file, unsigned int startaddr, unsigned int len, unsigned value) {
	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		exe->Process(makeReadCommand(index));
		try {
			if (std::stoi(file->ReadOutputFile("ssd_output.txt").substr(2, 10), nullptr, 16) != value) {

				return false;
			}
		}
		catch (std::exception e) {
			return false;
		}
	}

	return true;
}