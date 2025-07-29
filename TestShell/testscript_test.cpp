#include "gmock/gmock.h"
#include "testscript.h"
#include "test.h"

using namespace testing;

TEST(testscript, 1_FullWriteAndReadCompare) {
	MockProcessExecutor mock;
	TestScript script{&mock};

	EXPECT_CALL(mock, Process("copy")).Times(1);

	script.script1_FullWriteAndReadCompare();
}

TEST(testscript, 2_PartialLBAWrite) {
	NiceMock<MockProcessExecutor> mock;
	TestScript script{ &mock };

	// Loop 30
	EXPECT_CALL(mock, Process("write 0 0xAAAAAAA0")).Times(30);
	EXPECT_CALL(mock, Process("write 1 0xAAAAAAA1")).Times(30);
	EXPECT_CALL(mock, Process("write 2 0xAAAAAAA2")).Times(30);
	EXPECT_CALL(mock, Process("write 3 0xAAAAAAA3")).Times(30);
	EXPECT_CALL(mock, Process("write 4 0xAAAAAAA4")).Times(30);

	EXPECT_CALL(mock, Process("read 0"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA0));

	EXPECT_CALL(mock, Process("read 1"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA1));

	EXPECT_CALL(mock, Process("read 2"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA2));

	EXPECT_CALL(mock, Process("read 3"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA3));


	EXPECT_CALL(mock, Process("read 4"))
		.Times(30)
		.WillRepeatedly(Return(0xAAAAAAA4));


	script.script2_PartialLBAWrite();
}