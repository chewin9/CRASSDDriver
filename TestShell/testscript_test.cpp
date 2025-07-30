#include "gmock/gmock.h"
#include "testscript.h"
#include "test.h"

using namespace testing;

TEST(testscript, 1_FullWriteAndReadCompare) {
	testing::NiceMock<MockProcessExecutor> mock;
	TestScriptRunner script{&mock};

	EXPECT_TRUE(script.runScript("1_FullWriteAndReadCompare"));
}

TEST(testscript, 2_PartialLBAWriteCmdTestPass) {
	NiceMock<MockProcessExecutor> mock;
	TestScriptRunner script{ &mock };

	// Loop 30
	EXPECT_CALL(mock, Process("ssd.exe W 0 0xAAAAAAA0")).Times(30);
	EXPECT_CALL(mock, Process("ssd.exe W 1 0xAAAAAAA1")).Times(30);
	EXPECT_CALL(mock, Process("ssd.exe W 2 0xAAAAAAA2")).Times(30);
	EXPECT_CALL(mock, Process("ssd.exe W 3 0xAAAAAAA3")).Times(30);
	EXPECT_CALL(mock, Process("ssd.exe W 4 0xAAAAAAA4")).Times(30);

	EXPECT_CALL(mock, Process("ssd.exe R 0"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA0));

	EXPECT_CALL(mock, Process("ssd.exe R 1"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA1));

	EXPECT_CALL(mock, Process("ssd.exe R 2"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA2));

	EXPECT_CALL(mock, Process("ssd.exe R 3"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA3));


	EXPECT_CALL(mock, Process("ssd.exe R 4"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA4));


	EXPECT_EQ(true, script.runScript("2_PartialLBAWrite"));
}

TEST(testscript, 2_PartialLBAWriteCmdTestFail) {
	NiceMock<MockProcessExecutor> mock;
	TestScriptRunner script{ &mock };

	// Loop 30
	EXPECT_CALL(mock, Process("ssd.exe W 0 0xAAAAAAA0")).Times(1);
	EXPECT_CALL(mock, Process("ssd.exe W 1 0xAAAAAAA1")).Times(1);
	EXPECT_CALL(mock, Process("ssd.exe W 2 0xAAAAAAA2")).Times(1);
	EXPECT_CALL(mock, Process("ssd.exe W 3 0xAAAAAAA3")).Times(1);
	EXPECT_CALL(mock, Process("ssd.exe W 4 0xAAAAAAA4")).Times(1);

	EXPECT_CALL(mock, Process("ssd.exe R 0"))
		.Times(1)
		.WillRepeatedly(Return(0));

	EXPECT_CALL(mock, Process("ssd.exe R 1"))
		.Times(1)
		.WillRepeatedly(Return(0));

	EXPECT_CALL(mock, Process("ssd.exe R 2"))
		.Times(1)
		.WillRepeatedly(Return(0));

	EXPECT_CALL(mock, Process("ssd.exe R 3"))
		.Times(1)
		.WillRepeatedly(Return(0));


	EXPECT_CALL(mock, Process("ssd.exe R 4"))
		.Times(1)
		.WillRepeatedly(Return(0));


	EXPECT_EQ(false, script.runScript("2_PartialLBAWrite"));
}

TEST(testscript, 2_CmdTestPass) {
	NiceMock<MockProcessExecutor> mock;
	TestScriptRunner script{ &mock };

	// Loop 30
	EXPECT_CALL(mock, Process("ssd.exe W 0 0xAAAAAAA0")).Times(30);
	EXPECT_CALL(mock, Process("ssd.exe W 1 0xAAAAAAA1")).Times(30);
	EXPECT_CALL(mock, Process("ssd.exe W 2 0xAAAAAAA2")).Times(30);
	EXPECT_CALL(mock, Process("ssd.exe W 3 0xAAAAAAA3")).Times(30);
	EXPECT_CALL(mock, Process("ssd.exe W 4 0xAAAAAAA4")).Times(30);

	EXPECT_CALL(mock, Process("ssd.exe R 0"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA0));

	EXPECT_CALL(mock, Process("ssd.exe R 1"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA1));

	EXPECT_CALL(mock, Process("ssd.exe R 2"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA2));

	EXPECT_CALL(mock, Process("ssd.exe R 3"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA3));


	EXPECT_CALL(mock, Process("ssd.exe R 4"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA4));


	EXPECT_EQ(true, script.runScript("2_"));
}

TEST(testscript, 2_CmdTestFail) {
	NiceMock<MockProcessExecutor> mock;
	TestScriptRunner script{ &mock };

	// Loop 30
	EXPECT_CALL(mock, Process("ssd.exe W 0 0xAAAAAAA0")).Times(1);
	EXPECT_CALL(mock, Process("ssd.exe W 1 0xAAAAAAA1")).Times(1);
	EXPECT_CALL(mock, Process("ssd.exe W 2 0xAAAAAAA2")).Times(1);
	EXPECT_CALL(mock, Process("ssd.exe W 3 0xAAAAAAA3")).Times(1);
	EXPECT_CALL(mock, Process("ssd.exe W 4 0xAAAAAAA4")).Times(1);

	EXPECT_CALL(mock, Process("ssd.exe R 0"))
		.Times(1)
		.WillRepeatedly(Return(0));

	EXPECT_CALL(mock, Process("ssd.exe R 1"))
		.Times(1)
		.WillRepeatedly(Return(0));

	EXPECT_CALL(mock, Process("ssd.exe R 2"))
		.Times(1)
		.WillRepeatedly(Return(0));

	EXPECT_CALL(mock, Process("ssd.exe R 3"))
		.Times(1)
		.WillRepeatedly(Return(0));


	EXPECT_CALL(mock, Process("ssd.exe R 4"))
		.Times(1)
		.WillRepeatedly(Return(0));


	EXPECT_EQ(false, script.runScript("2_"));
}