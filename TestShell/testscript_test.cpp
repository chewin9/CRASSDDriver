#include "gmock/gmock.h"
#include "testscript.h"
#include "test.h"

TEST(testscript, 1_FullWriteAndReadCompare) {
	testing::NiceMock<MockProcessExecutor> mock;
	TestScriptRunner script{&mock};

	EXPECT_TRUE(script.runScript("1_FullWriteAndReadCompare"));
}
