#include "gmock/gmock.h"
#include "shell.h"
#include "process_executor.h"
#include <fstream>

class MockProcessExecutor : public ProcessExecutor {
public:
	MOCK_METHOD(void, Process, (const std::string& command), (override));
};

TEST(TS, TSRead01) {
	MockProcessExecutor mockExecutor;
	EXPECT_CALL(mockExecutor, Process("ssd_executor.exe"))
		.Times(2);

	Shell shell(&mockExecutor);
	std::ofstream file("ssd_output.txt");
	file << "0 0x12345678\n1 0x23456789\n";
	file.close();
	EXPECT_EQ(shell.read(0), "0x12345678");
	EXPECT_EQ(shell.read(1), "0x23456789");
	if (std::remove("ssd_output.txt") == 0) {
		std::cout << "File remove Success\n";
	}
}