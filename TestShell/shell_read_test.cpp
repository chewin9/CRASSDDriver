#include "gmock/gmock.h"
#include "shell_read.h"
#include "process_executor.h"
#include <fstream>
#include "test.h"

TEST(TS, TSRead01) {
	MockProcessExecutor mockExecutor;

	ShellRead readShell(&mockExecutor);

	EXPECT_CALL(mockExecutor, Process)
		.WillOnce([](const std::string& cmd) {
		// 파일 생성
		std::ofstream file("ssd_output.txt");
		file << "0 0x12345678\n1 0x23456789\n";
		file.close();
		// int 반환 (mock이므로 임의 값 반환)
		return 0;
			});

	EXPECT_EQ(readShell.read("READ 0"), "0x12345678");
	//파일 삭제
	if (std::remove("ssd_output.txt") == 0) {
		std::cout << "File remove Success\n";
	}
}