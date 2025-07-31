#include "testscriptfactory.h"
#include <string>
#include <exception>
#include <iostream>
#include <random>
#include <ctime>
#include <memory>
#include "File.h"

bool FullWriteAndReadCompare::Run(IProcessExecutor* exe, IFile* file) {
	//Script
	int value = 5;
	int start = 0;
	const int length = 5;

	PrintScriptEnter();

	for (start = 0; start < MAX_ADDR; start += length) {
		for (int index = start;index < start + length; index++) {
			WriteBlock(exe, start, length, value);
			if (ReadCompare(exe, file, start, length, value) == false) {
				PrintScriptExit(false);
				return false;
			}
		}
	}

	PrintScriptExit(true);

	return true;
}

bool PartialLBAWrite::Run(IProcessExecutor* exe, IFile* file)
{
	bool IsPass = true;

	PrintScriptEnter();

	for (int loopcount = 0; loopcount < MAX_LOOP_COUNT; loopcount++)
	{
		PartialBlockWrite(exe);

		IsPass = GetPartialReadAndCompareResult(exe, file);

		if (IsPass == false) break;
	}

	PrintScriptExit(IsPass);

	return IsPass;
}

bool PartialLBAWrite::GetPartialReadAndCompareResult(IProcessExecutor* exe, IFile* file)
{
	bool IsPass = true;

	for (int readcount = 0; readcount < MAX_TEST_AREA; ++readcount) {
		exe->Process(makeReadCommand(readcount));
		try {
			IsPass = IsPass && (stoi(file->ReadOutputFile("ssd_output.txt").substr(2, 10)) == std::stoul(value_list[readcount], nullptr, 16));
		}
		catch (std::exception e) {
			return false;
		}
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

bool WriteReadAging::Run(IProcessExecutor* exe, IFile* file) {
	//Script
	std::srand(std::time({}));
	unsigned int data = rand();

	PrintScriptEnter();

	for (int count = 0; count < 200; count++) {

		WriteBlock(exe, 0, 1, data);

		if (ReadCompare(exe, file, 0, 1, data) == false) {
			PrintScriptExit(false);
			return false;
		}

		WriteBlock(exe, 99, 1, data);
		if (ReadCompare(exe, file, 99, 1, data) == false) {
			PrintScriptExit(false);
			return false;
		}
	}

	PrintScriptExit(true);
	return true;
}

bool EraseAndWriteAging::Run(IProcessExecutor* exe, IFile* file) {
	EraseBlock(exe, 0, 3);

	for (int j = 0;j < 30; j++) {
		for (int i = 2; i < 98; i+=2) {
			WriteBlock(exe, i, 1, 5);
			WriteBlock(exe, i, 1, 9);
			EraseBlock(exe, i, 3);

			if (ReadCompare(exe, file, i, 3, 0) == false) {
				PrintScriptExit(false);
				return false;
			}
		}
	}

	PrintScriptExit(true);
	return true;
}

bool TestScriptFactory::isMatch(std::string input, std::string scriptname) {
	int pos;
	if((pos = input.find('_')) == std::string::npos) return false;

	if (pos == input.size() - 1)
	{
		return (input.substr(0, pos) == scriptname.substr(0, pos));
	}
	else {
		return (input == scriptname);
	}
}

std::shared_ptr<TestScript> TestScriptFactory::createTestScript(const std::string& scriptname, Logger& logger) {
	if (isMatch(scriptname, "0_Dummy") == true)  return std::make_shared<DummyScript>(scriptname, logger);
	if (isMatch(scriptname, "1_FullWriteAndReadCompare") == true) return std::make_shared<FullWriteAndReadCompare>(scriptname, logger);
	if (isMatch(scriptname, "2_PartialLBAWrite") == true) return std::make_shared<PartialLBAWrite>(scriptname, logger);
	if (isMatch(scriptname, "3_WriteReadAging") == true) return std::make_shared<WriteReadAging>(scriptname, logger);
	if (isMatch(scriptname, "4_EraseAndWriteAging") == true) return std::make_shared<EraseAndWriteAging>(scriptname, logger);
	return nullptr;
}
