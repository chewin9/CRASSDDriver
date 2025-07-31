#include "testscriptfactory.h"
#include <string>
#include <exception>
#include <iostream>
#include <random>
#include <ctime>
#include "File.h"

bool FullWriteAndReadCompare::Run(IProcessExecutor* exe, IFile* file) {
	//Script
	int value = 5;
	int start = 0;
	const int length = 5;

	for (start = 0; start < MAX_ADDR; start += length) {
		for (int index = start;index < start + length; index++) {
			WriteBlock(exe, start, length, value);
			if (ReadCompare(exe, file, start, length, value) == false) return false;
		}
	}

	return true;
}

bool PartialLBAWrite::Run(IProcessExecutor* exe, IFile* file)
{
	bool IsPass = true;

	for (int loopcount = 0; loopcount < MAX_LOOP_COUNT; loopcount++)
	{
		PartialBlockWrite(exe);

		IsPass = GetPartialReadAndCompareResult(exe, file);

		if (IsPass == false) break;
	}

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

	for (int count = 0; count < 200; count++) {

		WriteBlock(exe, 0, 1, data);
		if (ReadCompare(exe, file, 0, 1, data) == false) return false;

		WriteBlock(exe, 99, 1, data);
		if (ReadCompare(exe, file, 99, 1, data) == false) return false;
	}

	return true;
}

bool EraseAndWriteAging::Run(IProcessExecutor* exe, IFile* file) {
	EraseBlock(exe, 0, 3);

	for (int j = 0;j < 30; j++) {
		for (int i = 2; i < 98; i+=2) {
			WriteBlock(exe, i, 1, 5);
			WriteBlock(exe, i, 1, 9);
			EraseBlock(exe, i, 3);
			ReadCompare(exe, file, i, 3, 0);
		}
	}

	return true;
}

TestScript* TestScriptFactory::createTestScript(std::string scriptname) {
	if (scriptname == "0_Dummy") return new DummyScript(scriptname);
	if (scriptname == "1_FullWriteAndReadCompare") return new FullWriteAndReadCompare(scriptname);
	if (scriptname == "2_PartialLBAWrite") return new PartialLBAWrite(scriptname);
	if (scriptname == "3_WriteReadAging") return new WriteReadAging(scriptname);
	if (scriptname == "4_EraseAndWriteAging") return new EraseAndWriteAging(scriptname);

	return nullptr;
}