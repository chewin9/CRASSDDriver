#include "gmock/gmock.h"
#include "logger.h"
#include <exception>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdio>

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
	MOCK_METHOD(bool, is_saved_log_file_exists, (), (override));
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

	void create_dummy_file(const std::string& filename) {
		std::ofstream ofs(filename.c_str());  // or just std::ofstream ofs(filename);
		if (!ofs) {
			std::cerr << "Failed to create file: " << filename << std::endl;
		}
		// File is created and remains empty
		// ofs will automatically close when going out of scope
	}

};

TEST_F(LogTest, NormalLogOutput) {

	logger.print("Shell.release()", "hello!");

	EXPECT_THAT(oss.str(), Not(StrEq(""))); 
	EXPECT_THAT(filename, IsExistingFile());
	remove(filename.c_str());
}

TEST_F(LogTest, NoLogOutputOnConsole) {

	logger.disable_console_print();
	logger.print("Shell.release()", "hello!");

	EXPECT_THAT(oss.str(), StrEq("")); 
	EXPECT_THAT(filename, IsExistingFile());
	remove(filename.c_str());
}

TEST_F(LogTest, MoveFileWhenOver10K) {
	MockLogger logger;
	EXPECT_CALL(logger, is_file_over_10k).WillRepeatedly(Return(true));

	logger.print("Shell.release()", "hello!");

	std::string savedFile = logger.get_saved_log_file_name();

	EXPECT_THAT(filename, Not(IsExistingFile()));
	EXPECT_THAT(savedFile, IsExistingFile());
	remove(savedFile.c_str());
}

TEST_F(LogTest, MoveLogFileToZip) {
	MockLogger logger;
	EXPECT_CALL(logger, is_file_over_10k).WillRepeatedly(Return(true));
	EXPECT_CALL(logger, is_saved_log_file_exists).WillRepeatedly(Return(true));

	std::string testfile = "Testing.log";
	std::string testzipfile = "Testing.zip";

	create_dummy_file(testfile);
	logger.print("Shell.release()", "hello!");

	std::string savedFile = logger.get_saved_log_file_name();

	EXPECT_THAT(filename, Not(IsExistingFile()));
	EXPECT_THAT(testfile, Not(IsExistingFile()));
	EXPECT_THAT(savedFile, IsExistingFile());
	EXPECT_THAT(testzipfile, IsExistingFile());

	remove(savedFile.c_str());
	remove(testzipfile.c_str());
}
