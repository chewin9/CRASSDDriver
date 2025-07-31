#include "gmock/gmock.h"
#include "testscript.h"
#include "test.h"
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
				.Times(30)
				.WillRepeatedly(Return(readcount + 1));
		}
	}

	void ReadSetUpFail()
	{
		EXPECT_CALL(mock, Process(SSD_DRIVER_NAME + " R 0"))
			.Times(1)
			.WillRepeatedly(Return(0xFFFFFFFF));
	}

	void CheckResult(bool expected, std::string cmdline)
	{
		EXPECT_EQ(expected, script.runScript(cmdline));
	}
protected:
	NiceMock<MockProcessExecutor> mock;

private:
	TestScriptRunner script{ &mock };
	std::vector<std::string> value_list = { "0x00000001", "0x00000002", "0x00000003","0x00000004","0x00000005" };
	const int MAX_TEST_BLOCK = 5;
	const std::string SSD_DRIVER_NAME = "SSDDriver.exe";
};

TEST_F(TestScriptTestFixture, 1_FullWriteAndReadCompare) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(5));
	CheckResult(true, "1_FullWriteAndReadCompare");
}

TEST_F(TestScriptTestFixture, 1_FullWriteAndReadCompareShortType) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(5));
	CheckResult(true, "1_");
}

TEST_F(TestScriptTestFixture, 1_FullWriteAndReadCompareInvalidScript) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(5));
	CheckResult(false, "1_1");
}

TEST_F(TestScriptTestFixture, 1_FullWriteAndReadCompareScriptRunFail) {
	EXPECT_CALL(mock, Process(_)).WillRepeatedly(Return(4));
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
