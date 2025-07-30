#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "command_factory.h"
#include "command_parser.h"
#include "file_io.h"

class SSDCommandTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::remove("ssd_nand.txt");
    std::remove("ssd_output.txt");
  }

  void ClearTestFiles() {
    std::remove("ssd_nand.txt");
    std::remove("ssd_output.txt");
  }

  std::string ReadOutputFile() {
    std::ifstream in("ssd_output.txt");
    std::string line;
    std::getline(in, line);
    return line;
  }

  ParsedCommand MakeCommand(const std::vector<std::string>& args) {
    std::vector<char*> argv;
    for (auto& s : const_cast<std::vector<std::string>&>(args))
      argv.push_back(&s[0]);
    int argc = static_cast<int>(argv.size());
    CommandParser parser;
    return parser.ParseCommand(argc, argv.data());
  }

  void ExecuteCommand(ParsedCommand cmd) {
    ICommand* command = CommandFactory::create(cmd);
    ASSERT_NE(command, nullptr);
    command->Execute();
    delete command;
  }

  bool CheckNandFileContains(const std::string& expectedLine) {
    std::ifstream check("ssd_nand.txt");
    if (!check.is_open()) return false;
    std::string line;
    while (std::getline(check, line)) {
      if (line == expectedLine) return true;
    }
    return false;
  }
};

TEST_F(SSDCommandTest, FileWriteAndCheck) {
  ParsedCommand cmd = MakeCommand({"./SSDDriver.exe", "W", "42", "0xCAFEBABE"});
  ExecuteCommand(cmd);
  EXPECT_TRUE(CheckNandFileContains("42 0xCAFEBABE"));
}

TEST_F(SSDCommandTest, OverwriteSameLba) {
  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "W", "42", "0xAAAABBBB"}));
  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "W", "42", "0xCCCCDDDD"}));
  EXPECT_TRUE(CheckNandFileContains("42 0xCCCCDDDD"));
  EXPECT_FALSE(CheckNandFileContains("42 0xAAAABBBB"));
}

TEST_F(SSDCommandTest, OutOfRangeLBA) {
  ParsedCommand cmd =
      MakeCommand({"./SSDDriver.exe", "W", "999", "0xABCDEF01"});
  ASSERT_TRUE(cmd.errorFlag);
  ExecuteCommand(cmd);
  EXPECT_EQ(ReadOutputFile(), "ERROR");
}

TEST_F(SSDCommandTest, InvalidHexValue) {
  ParsedCommand cmd = MakeCommand({"./SSDDriver.exe", "W", "10", "INVALIDHEX"});
  ASSERT_TRUE(cmd.errorFlag);
  ExecuteCommand(cmd);
  EXPECT_EQ(ReadOutputFile(), "ERROR");
}

TEST_F(SSDCommandTest, WriteThenRead_ShouldMatchOutputFile) {
  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "W", "42", "0xCAFEBABE"}));
  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "R", "42"}));
  EXPECT_EQ(ReadOutputFile(), "0xCAFEBABE");
}

TEST_F(SSDCommandTest, InvalidLba_ShouldWriteErrorToOutputFile) {
  ParsedCommand cmd = MakeCommand({"./SSDDriver.exe", "R", "-1"});
  ASSERT_TRUE(cmd.errorFlag);
  ExecuteCommand(cmd);
  EXPECT_EQ(ReadOutputFile(), "ERROR");
}

TEST_F(SSDCommandTest, MultiWriteRead_ThirdValueShouldMatch) {
  ClearTestFiles();

  std::vector<std::pair<std::string, std::string>> writes = {
      {"0", "0xAAAABBBB"}, {"1", "0x11112222"},
      {"2", "0x33334444"},  // <== 우리가 read할 대상
      {"3", "0x55556666"}, {"4", "0x77778888"},
  };

  for (const auto& write : writes) {
    std::string lba = write.first;
    std::string value = write.second;

    std::vector<std::string> args = {"./SSDDriver.exe", "W", lba, value};
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

  std::vector<std::string> readArgs = {"./SSDDriver.exe", "R", "2"};
  std::vector<char*> readArgv;
  for (auto& s : readArgs) readArgv.push_back(&s[0]);
  int readArgc = static_cast<int>(readArgv.size());

  CommandParser parser;
  ParsedCommand readCmd = parser.ParseCommand(readArgc, readArgv.data());

  ICommand* readCommand = CommandFactory::create(readCmd);
  ASSERT_NE(readCommand, nullptr);
  readCommand->Execute();
  delete readCommand;

  std::string output = ReadOutputFile();
  EXPECT_EQ(output, "0x33334444");
}