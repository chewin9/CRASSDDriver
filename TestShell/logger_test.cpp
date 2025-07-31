#include "gmock/gmock.h"
#include "logger.h"
#include <exception>

using namespace testing;

MATCHER(IsExistingFile, "checks if file exists") {
	try {
		FILE* f = nullptr;
		errno_t err = fopen_s(&f, std::string(arg).c_str(), "r");
		if (f) fclose(f);
		return (err == 0) && (f != nullptr);
	}
	catch (const std::exception& e){
		return false;
	}
}

class MockLogger : public Logger {
public:
	MOCK_METHOD(bool, is_file_over_10k, (const std::string& file), (override));
};

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

TEST_F(LogTest, MoveFileWhenOver10K) {
	MockLogger logger;
	EXPECT_CALL(logger, is_file_over_10k).WillRepeatedly(Return(true));

	logger.print("Shell.release()", "hello!");

	std::string savedFile = logger.get_saved_log_file_name();

	EXPECT_THAT(filename, Not(IsExistingFile()));
	EXPECT_THAT(savedFile, IsExistingFile());
}
