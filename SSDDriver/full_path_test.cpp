#include <gtest/gtest.h>

#include <cstdio>  // remove
#include <fstream>
#include <string>
#include <vector>

#include "command_factory.h"
#include "command_parser.h"
#include "file_io.h"

TEST(FullPathTest, FileWriteAndCheck) {
  std::remove("ssd_nand.txt");

  std::vector<std::string> args = {"./SSDDriver.exe", "W", "42", "0xCAFEBABE"};

  std::vector<char*> argv;
  for (auto& s : args) argv.push_back(&s[0]);
  int argc = static_cast<int>(argv.size());

  CommandParser parser;
  ParsedCommand cmd = parser.ParseCommand(argc, argv.data());


  ICommand* writeCmd = CommandFactory::create(cmd);
  ASSERT_NE(writeCmd, nullptr);
  writeCmd->Execute();
  delete writeCmd;

  std::ifstream check("ssd_nand.txt");
  ASSERT_TRUE(check.is_open());

  std::string line;
  bool found = false;
  while (std::getline(check, line)) {
    if (line == "42 0xCAFEBABE") {
      found = true;
      break;
    }
  }

  EXPECT_TRUE(found) << "Expected entry not found in ssd_nand.txt";
  check.close();
}

TEST(OverwriteTest, OverwriteSameLba) {
  std::remove("ssd_nand.txt");
  {
    std::vector<std::string> args = {"./SSDDriver.exe", "W", "42",
                                     "0xAAAABBBB"};
    std::vector<char*> argv;
    for (auto& s : args) argv.push_back(&s[0]);
    int argc = static_cast<int>(argv.size());

    CommandParser parser;
    ParsedCommand cmd = parser.ParseCommand(argc, argv.data());

    ICommand* writeCmd = CommandFactory::create(cmd);
    ASSERT_NE(writeCmd, nullptr);
    writeCmd->Execute();
    delete writeCmd;
  }

  {
    std::vector<std::string> args = {"./SSDDriver.exe", "W", "42",
                                     "0xCCCCDDDD"};
    std::vector<char*> argv;
    for (auto& s : args) argv.push_back(&s[0]);
    int argc = static_cast<int>(argv.size());

    CommandParser parser;
    ParsedCommand cmd = parser.ParseCommand(argc, argv.data());

    ICommand* writeCmd = CommandFactory::create(cmd);
    ASSERT_NE(writeCmd, nullptr);
    writeCmd->Execute();
    delete writeCmd;
  }

  std::ifstream check("ssd_nand.txt");
  ASSERT_TRUE(check.is_open());

  std::string line;
  bool found = false;
  while (std::getline(check, line)) {
    if (line == "42 0xCCCCDDDD") {
      found = true;
    }

    ASSERT_NE(line, "42 0xAAAABBBB")
        << "Old value should have been overwritten!";
  }

  EXPECT_TRUE(found) << "Overwritten value not found!";
  check.close();
}

void ClearOutputFile() { std::remove("ssd_output.txt"); }

std::string ReadOutputFile() {
  std::ifstream in("ssd_output.txt");
  std::string line;
  std::getline(in, line);
  return line;
}

TEST(ErrorInputTest, OutOfRangeLBA) {
  ClearOutputFile();

  std::vector<std::string> args = {"./SSDDriver.exe", "W", "999", "0xABCDEF01"};
  std::vector<char*> argv;
  for (auto& s : args) argv.push_back(&s[0]);
  int argc = static_cast<int>(argv.size());

  CommandParser parser;
  ParsedCommand cmd = parser.ParseCommand(argc, argv.data());

  ASSERT_TRUE(cmd.errorFlag);  // 에러 플래그가 켜졌는지 확인

  ICommand* writeCmd = CommandFactory::create(cmd);
  ASSERT_NE(writeCmd, nullptr);
  writeCmd->Execute();
  delete writeCmd;

  std::string output = ReadOutputFile();
  EXPECT_EQ(output, "ERROR");
}

TEST(ErrorInputTest, InvalidHexValue) {
  ClearOutputFile();

  std::vector<std::string> args = {"./SSDDriver.exe", "W", "10", "INVALIDHEX"};
  std::vector<char*> argv;
  for (auto& s : args) argv.push_back(&s[0]);
  int argc = static_cast<int>(argv.size());

  CommandParser parser;
  ParsedCommand cmd = parser.ParseCommand(argc, argv.data());

  ASSERT_TRUE(cmd.errorFlag);

  ICommand* writeCmd = CommandFactory::create(cmd);
  ASSERT_NE(writeCmd, nullptr);
  writeCmd->Execute();
  delete writeCmd;

  std::string output = ReadOutputFile();
  EXPECT_EQ(output, "ERROR");
}


void ClearTestFiles() {
  std::remove("ssd_nand.txt");
  std::remove("ssd_output.txt");
}


TEST(IntegrationTest, WriteThenRead_ShouldMatchOutputFile) {
  ClearTestFiles();

  // Step 1: Write to LBA 42 with value 0xCAFEBABE
  {
    std::vector<std::string> args = {"./SSDDriver.exe", "W", "42",
                                     "0xCAFEBABE"};
    std::vector<char*> argv;
    for (auto& s : args) argv.push_back(&s[0]);
    int argc = static_cast<int>(argv.size());

    CommandParser parser;
    ParsedCommand cmd = parser.ParseCommand(argc, argv.data());

    ASSERT_FALSE(cmd.errorFlag);

    ICommand* writeCmd = CommandFactory::create(cmd);
    ASSERT_NE(writeCmd, nullptr);
    writeCmd->Execute();
    delete writeCmd;
  }

  // Step 2: Read from LBA 42
  {
    std::vector<std::string> args = {"./SSDDriver.exe", "R", "42"};
    std::vector<char*> argv;
    for (auto& s : args) argv.push_back(&s[0]);
    int argc = static_cast<int>(argv.size());

    CommandParser parser;
    ParsedCommand cmd = parser.ParseCommand(argc, argv.data());

    ASSERT_FALSE(cmd.errorFlag);

    ICommand* readCmd = CommandFactory::create(cmd);
    ASSERT_NE(readCmd, nullptr);
    readCmd->Execute();
    delete readCmd;
  }

  // Step 3: Check the output file
  std::string output = ReadOutputFile();
  EXPECT_EQ(output, "0xCAFEBABE");
}


TEST(ReadCommandTest, InvalidLba_ShouldWriteErrorToOutputFile) {
  ClearTestFiles();

  std::vector<std::string> args = {"./SSDDriver.exe", "R",
                                   "-1"};  // 잘못된 LBA (-1)
  std::vector<char*> argv;
  for (auto& s : args) argv.push_back(&s[0]);
  int argc = static_cast<int>(argv.size());

  CommandParser parser;
  ParsedCommand cmd = parser.ParseCommand(argc, argv.data());

  EXPECT_TRUE(cmd.errorFlag);  // 파싱 시 에러 감지되어야 함

  ICommand* readCmd = CommandFactory::create(cmd);
  ASSERT_NE(readCmd, nullptr);
  readCmd->Execute();  // 에러 발생 시 ssd_output.txt에 "ERROR" 기록해야 함
  delete readCmd;

  std::string output = ReadOutputFile();
  EXPECT_EQ(output, "ERROR");
}