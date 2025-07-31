#include "gmock/gmock.h"
#include "logger.h"
using namespace testing;

MATCHER(IsExistingFile, "checks if file exists") {
	FILE* f = nullptr;
	errno_t err = fopen_s(&f, std::string(arg).c_str(), "r");
	if (f) fclose(f); 
    return (err == 0) && (f != nullptr);
}

class LogTest : public Test {
public:
	void SetUp() override {
		oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());
	}
	
	void TearDown() override {
		std::cout.rdbuf(oldCoutStreamBuf);
		//std::cout <<"result: "<< oss.str() << std::endl;
	}
	Logger logger;
	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf;
	std::string filename = "latest.log";
};

TEST_F(LogTest, NormalLogOutput) {

	logger.print("Shell.release()", "hello!");

	EXPECT_THAT(oss.str(), Not(StrEq(""))); 
	EXPECT_THAT(filename, IsExistingFile());
}

TEST_F(LogTest, NoLogOutputOnConsole) {

	logger.disable_console_print();
	logger.print("Shell.release()", "hello!");

	EXPECT_THAT(oss.str(), StrEq("")); 
	EXPECT_THAT(filename, IsExistingFile());
}

