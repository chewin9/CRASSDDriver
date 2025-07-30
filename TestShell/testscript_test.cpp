#include "gmock/gmock.h"
#include "testscript.h"
#include "test.h"

TEST(testscript, 1_FullWriteAndReadCompare) {
	MockProcessExecutor mock;
	TestScriptRunner script{&mock};

	EXPECT_CALL(mock, Process("copy")).Times(1);

	script.runScript("1_FullWriteAndReadCompare");
}
