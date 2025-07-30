#include "testscript.h"
#include <string>
#include <exception>
#include <iostream>
#include <random>
#include <ctime>
#include <fstream>

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

bool TestScriptRunner::IsValidSciprtCommand(const std::string& commandLine) {
	if (parseCommandLine(commandLine) == INVALID_INDEX) {
		return false;
	}

	return true;
}

void TestScriptRunner::addScripts() {
	testScripts.push_back(new DummyScript("0_Dummy"));
	testScripts.push_back(new FullWriteAndReadCompare("1_FullWriteAndReadCompare"));
	testScripts.push_back(new PartialLBAWrite("2_PartialLBAWrite"));
	testScripts.push_back(new WriteReadAging("3_WriteReadAging"));
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

void TestScript::WriteBlock(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned int value) {
	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		exe->Process(makeWriteCommand(index, value));
	}
}

bool TestScript::ReadCompare(IProcessExecutor* exe, unsigned int startaddr, unsigned int len, unsigned value) {
	for (unsigned int index = startaddr; index < startaddr + len; index++) {
		exe->Process(makeReadCommand(index));
		if (std::stoi(ReadOutputFile().substr(2, 10), nullptr, 16) != value) {
			
			return false;
		}
	}

	return true;
}

std::string TestScript::ReadOutputFile(const std::string& filename) {
	std::string output;
	std::ifstream input(filename);
	if (!input.is_open()) {
		std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
		return output;
	}

	std::getline(input, output);
	input.close();
	return output;
}

bool FullWriteAndReadCompare::Run(IProcessExecutor* exe) {
	//Script
	int value = 5;
	int start = 0;
	const int length = 5;

	for (start = 0; start < MAX_ADDR; start += length) {
		for (int index = start;index < start + length; index++) {
			WriteBlock(exe, start, length, value);
			if (ReadCompare(exe, start, length, value) == false) return false;
		}
	}

	return true;
}

bool PartialLBAWrite::Run(IProcessExecutor* exe)
{
	bool IsPass = true;

	for (int loopcount = 0; loopcount < MAX_LOOP_COUNT; loopcount++)
	{
		PartialBlockWrite(exe);

		IsPass = GetPartialReadAndCompareResult(exe);

		if (IsPass == false) break;
	}

	return IsPass;
}

bool PartialLBAWrite::GetPartialReadAndCompareResult(IProcessExecutor* exe)
{
	bool IsPass = true;

	for (int readcount = 0; readcount < MAX_TEST_AREA; ++readcount) {
		exe->Process(makeReadCommand(readcount));
		IsPass = IsPass && (stoi(ReadOutputFile().substr(2, 10)) == std::stoul(value_list[readcount], nullptr, 16));
	}

	return IsPass;
}

void PartialLBAWrite::PartialBlockWrite(IProcessExecutor* exe)
{
	for (int writecount = 0; writecount < MAX_TEST_AREA; writecount++)
	{
		exe->Process(makeWriteCommand(writecount, std::stoul(value_list[writecount], nullptr, 16)));
	}
}

bool WriteReadAging::Run(IProcessExecutor* exe) {
	//Script
	std::srand(std::time({}));
	unsigned int data = rand();

	for (int count = 0; count < 200; count++) {
		
		WriteBlock(exe, 0, 1, data);
		if (ReadCompare(exe, 0, 1, data) == false) return false;

		WriteBlock(exe, 99, 1, data);
		if (ReadCompare(exe, 99, 1, data) == false) return false;
	}

	return true;
}

