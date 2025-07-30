#include "gmock/gmock.h"
#include "testscript.h"
#include "test.h"
#include <vector>

using namespace testing;

class PartialLBAWriteFixture : public Test
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

	void ReadSetUpPass()
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

	void CheckResult(int expected, std::string cmdline)
	{
		EXPECT_EQ(expected, script.runScript(cmdline));
	}
private:
	NiceMock<MockProcessExecutor> mock;
	TestScriptRunner script{ &mock };
	std::vector<std::string> value_list = { "0x1", "0x2", "0x3","0x4","0x5" };
	const int MAX_TEST_BLOCK = 5;
	const std::string SSD_DRIVER_NAME = "ssd.exe";
};

TEST(testscript, 1_FullWriteAndReadCompare) {
	testing::NiceMock<MockProcessExecutor> mock;
	TestScriptRunner script{&mock};

	EXPECT_TRUE(script.runScript("1_FullWriteAndReadCompare"));
}

TEST_F(PartialLBAWriteFixture, 2_PartialLBAWriteCmdTestPass) 
{
	PartialWriteSetup(30);
	ReadSetUpPass();
	CheckResult(true, "2_PartialLBAWrite");
}

TEST_F(PartialLBAWriteFixture, 2_PartialLBAWriteCmdTestFail) 
{
	PartialWriteSetup(1);
	ReadSetUpFail();
	CheckResult(false, "2_PartialLBAWrite");
}

TEST_F(PartialLBAWriteFixture, 2_CmdTestPass) {
	PartialWriteSetup(30);
	ReadSetUpPass();
	CheckResult(true, "2_");
}

TEST_F(PartialLBAWriteFixture, 2_CmdTestFail) {
	PartialWriteSetup(1);
	ReadSetUpFail();
	CheckResult(false, "2_");
}