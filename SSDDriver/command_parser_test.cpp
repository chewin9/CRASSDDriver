#include "command_parser.h"

#include "gmock/gmock.h"

TEST(CommandParserTest, ParserNormalWriteCase) {
  CommandParser cp;
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13",
                  (char*)"0xFFFFFFFF"};
  ParsedCommand cmd = cp.ParseCommand(4, argv);
  EXPECT_EQ(cmd.lba, 13);
  EXPECT_EQ(cmd.opCode, "W");
  EXPECT_EQ(cmd.value, "0xFFFFFFFF");
  EXPECT_FALSE(cmd.errorFlag);
}

TEST(CommandParserTest, ParserNormalReadCase) {
  CommandParser cp;
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"R", (char*)"13" };
  ParsedCommand cmd = cp.ParseCommand(3, argv);
  EXPECT_EQ(cmd.lba, 13);
  EXPECT_EQ(cmd.opCode, "R");
  EXPECT_EQ(cmd.value, "");
  EXPECT_FALSE(cmd.errorFlag);
}

TEST(CommandParserTest, ParserWithWriteExceptionCase) {
  CommandParser cp;
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13"};
  EXPECT_THROW({ cp.ParseCommand(3, argv); }, std::invalid_argument);
}

TEST(CommandParserTest, ParserWithReadExceptionCase) {
  CommandParser cp;
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"R", (char*)"13", (char*)"0xFFFFFFFF"};
  EXPECT_THROW({ cp.ParseCommand(4, argv); }, std::invalid_argument);
}

TEST(CommandParserTest, ParserWithExceptionCase) {
  CommandParser cp;
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"R"};
  EXPECT_THROW({ cp.ParseCommand(2, argv); }, std::invalid_argument);
}

TEST(CommandParserTest, ParserWithInvalidParameter1) {
  CommandParser cp;
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"100",
                  (char*)"0xFFFFFFFF"};
  ParsedCommand cmd = cp.ParseCommand(4, argv);
  EXPECT_TRUE(cmd.errorFlag);
}

TEST(CommandParserTest, ParserWithInvalidParameter2) {
  CommandParser cp;
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13",
                  (char*)"0x123"};
  ParsedCommand cmd = cp.ParseCommand(4, argv);
  EXPECT_TRUE(cmd.errorFlag);
}
TEST(CommandParserTest, ParserWithInvalidParameter3) {
  CommandParser cp;
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13",
                  (char*)"11"};
  ParsedCommand cmd = cp.ParseCommand(4, argv);
  EXPECT_TRUE(cmd.errorFlag);
}

TEST(CommandParserTest, ParserWithInvalidParameter4) {
  CommandParser cp;
  char* argv[] = {(char*)"SSDDriver.exe", (char*)"W", (char*)"13", (char*)"0xABCDEFGH"};
  ParsedCommand cmd = cp.ParseCommand(4, argv);
  EXPECT_TRUE(cmd.errorFlag);
}