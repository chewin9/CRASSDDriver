#include "gmock/gmock.h"
#include "Shell.h"
#include "test.h"
#include "shell_write.h"

using namespace testing;

class MockShell : public Shell {
public:
	MockShell(IProcessExecutor* exe) : Shell(exe) {};
	MOCK_METHOD(string, get_command, (string& input), (override));
};

TEST(ShellTest, CallCommandWrite) {
	MockProcessExecutor mock_exe;
	MockShell mockshell(&mock_exe);

	EXPECT_CALL(mockshell, get_command).WillOnce([](string& input) { input = "write 3 0xAAAABBBB";  return "write"; })
									   .WillRepeatedly([](string& input) { return "exit"; });
	EXPECT_CALL(mock_exe, Process("SSDDriver.exe W 3 0xAAAABBBB")).Times(1);

	mockshell.Run();
}

TEST(ShellTest, CallCommandWriteFull) {
	MockProcessExecutor mock_exe;
	MockShell mockshell(&mock_exe);

	EXPECT_CALL(mockshell, get_command).WillOnce([](string& input) { input = "fullwrite 0xAAAABBBB";  return "fullwrite"; })
		.WillRepeatedly([](string& input) { return "exit"; });
	EXPECT_CALL(mock_exe, Process).Times(100);

	mockshell.Run();
}

TEST(ShellTest, CallCommandRead) {
	MockProcessExecutor mock_exe;
	MockShell mockshell(&mock_exe);

	EXPECT_CALL(mockshell, get_command).WillOnce([](string& input) { input = "read 3";  return "read"; })
		.WillRepeatedly([](string& input) { return "exit"; });
	EXPECT_CALL(mock_exe, Process("SSDDriver.exe R 3")).Times(1);

	mockshell.Run();
}
