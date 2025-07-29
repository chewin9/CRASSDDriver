#include "gmock/gmock.h"
#include "testscript.h"
#include "test.h"

TEST(testscript, 1_FullWriteAndReadCompare) {
	MockProcessExecutor mock;
	TestScript script{&mock};

	EXPECT_CALL(mock, Process("copy")).Times(1);

	script.script1_FullWriteAndReadCompare();
}
