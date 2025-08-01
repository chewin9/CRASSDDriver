#include "testscriptfactory.h"
#include <string>
#include <exception>
#include <iostream>
#include <random>
#include <ctime>
#include <memory>
#include "File.h"
#include "testscript_util.h"
#include "testscriptlog.h"

const int HEX = 16;

bool FullWriteAndReadCompare::Run(IProcessExecutor* exe, IFile* file) {
	//Script
	int value = 5;
	int start = 0;
	const int length = 5;

	PRINT("Start");

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

	PRINT("Pass");
	PrintScriptExit(true);

	return true;
}

bool PartialLBAWrite::Run(IProcessExecutor* exe, IFile* file)
{
	bool IsPass = true;

	PRINT("start");

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

	PRINT("");

	for (int readcount = 0; readcount < MAX_TEST_AREA; ++readcount) {
		exe->Process(makeReadCommand(readcount));
		try {
			IsPass = IsPass && (stoul(file->ReadOutputFile("ssd_output.txt").substr(2, 10)) == std::stoul(value_list[readcount], nullptr, HEX));
		}
		catch (std::exception e) {
			PRINT("Exception");
			return false;
		}
	}

	PRINT(((IsPass) ? "PASS" : "FAIL"));
	return IsPass;
}

void PartialLBAWrite::PartialBlockWrite(IProcessExecutor* exe)
{
	for (int writecount = 0; writecount < MAX_TEST_AREA; writecount++)
	{
		exe->Process(makeWriteCommand(writecount, std::stoul(value_list[writecount], nullptr, HEX)));
	}
}

bool WriteReadAging::Run(IProcessExecutor* exe, IFile* file) {
	//Script
	PRINT("Start");

	PrintScriptEnter();

	unsigned int data = TestScriptUtil::GetInstance().GetRandomValue();

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

	PRINT("Fail");
	PrintScriptExit(true);
	return true;
}

bool EraseAndWriteAging::Run(IProcessExecutor* exe, IFile* file) {
	PRINT("Start");

	PrintScriptEnter();

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

	PRINT("Pass");

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


std::shared_ptr<TestScript> TestScriptFactory::createTestScript(const std::string& scriptname) {
	if (isMatch(scriptname, "1_FullWriteAndReadCompare") == true) return std::make_shared<FullWriteAndReadCompare>("1_FullWriteAndReadCompare");
	if (isMatch(scriptname, "2_PartialLBAWrite") == true) return std::make_shared<PartialLBAWrite>("2_PartialLBAWrite");
	if (isMatch(scriptname, "3_WriteReadAging") == true) return std::make_shared<WriteReadAging>("3_WriteReadAging");
	if (isMatch(scriptname, "4_EraseAndWriteAging") == true) return std::make_shared<EraseAndWriteAging>("4_EraseAndWriteAging");

	return nullptr;
}
