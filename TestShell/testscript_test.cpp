#include "gmock/gmock.h"
#include "testscript.h"
#include "test.h"
#include "File.h"
#include <vector>

using namespace testing;

class TestScriptTestFixture : public Test
{
public:
	void PartialWriteSetup(int callcount)
	{
		for (int writecount = 0; writecount < MAX_TEST_BLOCK; writecount++)
		{
			std::string command = SSD_DRIVER_NAME + " W " + std::to_string(writecount) + " " + value_list[writecount];
			EXPECT_CALL(mock, Process(command)).Times(callcount);
		}
	}

	void PartialReadSetUp()
	{
		for (int readcount = 0; readcount < MAX_TEST_BLOCK; ++readcount) {
			std::string command = SSD_DRIVER_NAME + " R " + std::to_string(readcount);
			EXPECT_CALL(mock, Process(command))
				.Times(30);
		}

		count = 0;

		EXPECT_CALL(mockfile, ReadOutputFile(_))
			.Times(150)  // 정확한 호출 횟수
			.WillRepeatedly(Invoke([&]() {
			int value = (count % 5) + 1;  // 1 → 5 반복
			count++;

			std::string temp = "0x0000000" + std::to_string(value);
			return temp;
				}));

	}

	void ReadSetUpFail()
	{
		EXPECT_CALL(mock, Process(SSD_DRIVER_NAME + " R 0"))
			.Times(1)
			.WillRepeatedly(Return(0xFFFFFFFF));
		EXPECT_CALL(mockfile, ReadOutputFile(_))
			.WillRepeatedly(Return(INVALID_VALUE_STRING));
	}

	void CheckResult(bool expected, std::string cmdline)
	{
		EXPECT_EQ(expected, script.runScript(cmdline));
	}

	void SetUpReadOutputReapeat(std::string value)
	{
		EXPECT_CALL(mockfile, ReadOutputFile(_)).WillRepeatedly(Return(value));
	}
	int count = 0;
protected:
	NiceMock<MockProcessExecutor> mock;
	NiceMock<MockFile> mockfile;
	const std::string INVALID_VALUE_STRING = "0xFFFFFFFF";
	const std::string ERASE_VALUE_STRING = "0x00000000";

private:
	TestScriptRunner script{ &mock,&mockfile };
	std::vector<std::string> value_list = { "0x00000001", "0x00000002", "0x00000003","0x00000004","0x00000005" };
	const int MAX_TEST_BLOCK = 5;
	const std::string SSD_DRIVER_NAME = "SSDDriver.exe";
};

TEST_F(TestScriptTestFixture, 1_FullWriteAndReadCompare) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(5));
	SetUpReadOutputReapeat("0x00000005");
	CheckResult(true, "1_FullWriteAndReadCompare");
}

TEST_F(TestScriptTestFixture, 1_FullWriteAndReadCompareShortType) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(5));
	SetUpReadOutputReapeat("0x00000005");
	CheckResult(true, "1_");
}

TEST_F(TestScriptTestFixture, 1_FullWriteAndReadCompareInvalidScript) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(5));
	CheckResult(false, "1_1");
}

TEST_F(TestScriptTestFixture, 1_FullWriteAndReadCompareScriptRunFail) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(4));
	SetUpReadOutputReapeat(INVALID_VALUE_STRING);
	CheckResult(false, "1_");
}

TEST_F(TestScriptTestFixture, 2_PartialLBAWriteCmdTestPass)
{
	PartialWriteSetup(30);
	PartialReadSetUp();
	CheckResult(true, "2_PartialLBAWrite");
}

TEST_F(TestScriptTestFixture, 2_PartialLBAWriteCmdTestFail)
{
	PartialWriteSetup(1);
	ReadSetUpFail();
	CheckResult(false, "2_PartialLBAWrite");
}

TEST_F(TestScriptTestFixture, 2_CmdTestPass) {
	PartialWriteSetup(30);
	PartialReadSetUp();
	CheckResult(true, "2_");
}

TEST_F(TestScriptTestFixture, 2_CmdTestFail) {
	PartialWriteSetup(1);
	ReadSetUpFail();
	CheckResult(false, "2_");
}

TEST_F(TestScriptTestFixture, DISABLED_3_WriteReadAgingNormal) {
	CheckResult(true, "3_WriteReadAgingNormal");
}

TEST_F(TestScriptTestFixture, DISABLED_3_WriteReadAgingShort) {
	CheckResult(true, "3_");
}

TEST_F(TestScriptTestFixture, 3_WriteReadAgingNotMatch) {
	CheckResult(false, "3_3");
}

TEST_F(TestScriptTestFixture, 4_EraseAndWriteAgingPass) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(0));
	SetUpReadOutputReapeat(ERASE_VALUE_STRING);
	CheckResult(true, "4_EraseAndWriteAging");
}

TEST_F(TestScriptTestFixture, 4_EraseAndWriteAgingFail) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(0xA));
	SetUpReadOutputReapeat(INVALID_VALUE_STRING);
	CheckResult(false, "4_EraseAndWriteAging");
}

TEST_F(TestScriptTestFixture, 4_CmdTestPass) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(0));
	SetUpReadOutputReapeat(ERASE_VALUE_STRING);
	CheckResult(true, "4_");
}

TEST_F(TestScriptTestFixture, 4_CmdTestFail) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(0));
	SetUpReadOutputReapeat(INVALID_VALUE_STRING);
	CheckResult(false, "4_");
}