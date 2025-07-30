#include "command_parser.h"

#include "gmock/gmock.h"
using namespace testing;

class CommandParserTestFixture : public Test {
 public:
  CommandParser cp;
  void CheckAssert(int argc, char* argv[]) {
    EXPECT_THROW({ cp.ParseCommand(argc, argv); }, std::invalid_argument);
  }

  void CheckErrorFlagOn(int argc, char* argv[]) {
    ParsedCommand cmd = cp.ParseCommand(argc, argv);
    EXPECT_TRUE(cmd.errorFlag);
  }
};

TEST_F(CommandParserTestFixture, ParserNormalWriteCase) {
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13",
                  (char*)"0xFFFFFFFF"};
  ParsedCommand cmd = cp.ParseCommand(4, argv);
  EXPECT_EQ(cmd.lba, 13);
  EXPECT_EQ(cmd.opCode, "W");
  EXPECT_EQ(cmd.value, "0xFFFFFFFF");
  EXPECT_FALSE(cmd.errorFlag);
}

TEST_F(CommandParserTestFixture, ParserNormalReadCase) {
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"R", (char*)"13"};
  ParsedCommand cmd = cp.ParseCommand(3, argv);
  EXPECT_EQ(cmd.lba, 13);
  EXPECT_EQ(cmd.opCode, "R");
  EXPECT_EQ(cmd.value, "");
  EXPECT_FALSE(cmd.errorFlag);
}

TEST_F(CommandParserTestFixture, ParserWithWriteExceptionCase) {
  int argCnt = 3;
  char* exceptionArgv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13"};
  CheckAssert(argCnt, exceptionArgv);
}

TEST_F(CommandParserTestFixture, ParserWithReadExceptionCase) {
  int argCnt = 4;
  char* exceptionArgv[] = {(char*)"SSDDriver.exe", (char*)"R", (char*)"13",
                           (char*)"0xFFFFFFFF"};
  CheckAssert(argCnt, exceptionArgv);
}

TEST_F(CommandParserTestFixture, ParserWithExceptionCase) {
  int argCnt = 2;
  char* exceptionArgv[] = {(char*)"SSDDriver.exe", (char*)"R"};
  CheckAssert(argCnt, exceptionArgv);
}


TEST_F(CommandParserTestFixture, ParserWithInvalidParameterWithOutofRangeLba) {
  int argCnt = 4;
  char* invalidArgv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"100",
                  (char*)"0xFFFFFFFF"};
  CheckErrorFlagOn(argCnt, invalidArgv);
}

TEST_F(CommandParserTestFixture, ParserWithInvalidParameterWithNotEnoughVal) {
  int argCnt = 4;
  char* invalidArgv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13",
                  (char*)"0x123"};
  CheckErrorFlagOn(argCnt, invalidArgv);
}

TEST_F(CommandParserTestFixture, ParserWithInvalidParameterWithDecimalVal) {
  int argCnt = 4;
  char* invalidArgv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13",
                  (char*)"11"};
  CheckErrorFlagOn(argCnt, invalidArgv);
}

TEST_F(CommandParserTestFixture, ParserWithInvalidParameterWithNoHexNot) {
  int argCnt = 4;
  char* invalidArgv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13",
                         (char*)"0xABCDEFGH"};
  CheckErrorFlagOn(argCnt, invalidArgv);
}