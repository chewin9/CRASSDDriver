#include "gmock/gmock.h"
#include "logger.h"
using namespace testing;

MATCHER(IsExistingFile, "checks if file exists") {
	FILE* f = nullptr;
	errno_t err = fopen_s(&f, std::string(arg).c_str(), "r");
	if (f) fclose(f); 
    return (err == 0) && (f != nullptr);
}

TEST(LogTest, NormalLogOutput) {
	Logger logger;
	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;
	oldCoutStreamBuf = std::cout.rdbuf();
	std::cout.rdbuf(oss.rdbuf());

	std::string filename = "latest.log";

	logger.print("Shell.release()", "hello!");

	// 1. console print
	EXPECT_THAT(oss.str(), Not(StrEq(""))); // no print 
	// 2. file output generation with .log expansion
	EXPECT_THAT(filename, IsExistingFile());
	std::cout.rdbuf(oldCoutStreamBuf);
}

TEST(LogTest, NoLogOutputOnConsole) {
	Logger logger;

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;

	oldCoutStreamBuf = std::cout.rdbuf();
	std::cout.rdbuf(oss.rdbuf());

	std::string filename = "latest.log";

	logger.disable_console_print();

	logger.print("Shell.release()", "hello!");
	// 1. console print should not happen
	EXPECT_THAT(oss.str(), StrEq("")); // no print 
	// 2. file output generation with .log expansion
	EXPECT_THAT(filename, IsExistingFile());

	std::cout.rdbuf(oldCoutStreamBuf);
}

