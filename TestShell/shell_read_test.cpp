#include "gmock/gmock.h"
#include "shell.h"
#include "process_executor.h"

class MockProcessExecutor : public ProcessExecutor {
public:
    MOCK_METHOD(void, readExecutor, (const std::string& command), (override));
};

TEST(TS, TSRead01) {
	MockProcessExecutor mockExecutor;
	EXPECT_CALL(mockExecutor, readExecutor("aa.exe"))
		.Times(2);

	Shell shell(&mockExecutor);

	EXPECT_EQ(shell.read(0), "0x12345678");
	EXPECT_EQ(shell.read(1), "0x23456789");
}