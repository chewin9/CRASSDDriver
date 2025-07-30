#include "gmock/gmock.h"
#include "testscript.h"
#include "test.h"

using namespace testing;

TEST(testscript, 1_FullWriteAndReadCompare) {
	MockProcessExecutor mock;
	TestScriptRunner script{&mock};

	EXPECT_CALL(mock, Process("copy")).Times(1);

	script.runScript("1_FullWriteAndReadCompare");
}

TEST(testscript, 2_PartialLBAWrite) {
	NiceMock<MockProcessExecutor> mock;
	TestScript script{ &mock };

	// Loop 30
	EXPECT_CALL(mock, Process("W 0 0xAAAAAAA0")).Times(30);
	EXPECT_CALL(mock, Process("W 1 0xAAAAAAA1")).Times(30);
	EXPECT_CALL(mock, Process("W 2 0xAAAAAAA2")).Times(30);
	EXPECT_CALL(mock, Process("W 3 0xAAAAAAA3")).Times(30);
	EXPECT_CALL(mock, Process("W 4 0xAAAAAAA4")).Times(30);

	EXPECT_CALL(mock, Process("R 0"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA0));

	EXPECT_CALL(mock, Process("R 1"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA1));

	EXPECT_CALL(mock, Process("R 2"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA2));

	EXPECT_CALL(mock, Process("R 3"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA3));


	EXPECT_CALL(mock, Process("R 4"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA4));


	EXPECT_EQ(true, script.script2_PartialLBAWrite());
}