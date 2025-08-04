#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "command_buffer.h"
#include "command_factory.h"
#include "command_parser.h"
#include "command_runner.h"
#include "file_io.h"
#include "ssd_operation_handler.h"

class SSDCommandTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::remove("ssd_nand.txt");
    std::remove("ssd_output.txt");

    // Command buffer directory 초기화
    system("rm -rf command_buffer");
    system("mkdir command_buffer");
  }

  void TearDown() override { ClearTestFiles(); }

  void ClearTestFiles() {
    std::remove("ssd_nand.txt");
    std::remove("ssd_output.txt");
    system("rm -rf command_buffer");
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
      std::unique_ptr<ICommandOptimizer> opt;
      if (cmd.opCode == "W") {
          opt = std::make_unique<WriteCommandOptimizer>();
      }
      else if (cmd.opCode == "E") {
          opt = std::make_unique<EraseCommandOptimizer>();
      }

      CommandBuffer cmdbuffer(std::move(opt));
    SsdOperationHandler opHandler(fileio, cmdbuffer);
    std::unique_ptr<ICommand> command = CommandFactory::create(cmd.opCode, opHandler);
    ASSERT_NE(command, nullptr);
    command->Execute(cmd);
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

  FileIO fileio;
};

TEST_F(SSDCommandTest, FileWriteAndCheck) {
  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "W", "42", "0xCAFEBABE"}));
  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "R", "42"}));
  EXPECT_EQ(ReadOutputFile(), "0xCAFEBABE");
}

TEST_F(SSDCommandTest, OverwriteSameLba) {
  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "W", "42", "0xAAAABBBB"}));
  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "W", "42", "0xCCCCDDDD"}));
  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "R", "42"}));
  EXPECT_EQ(ReadOutputFile(), "0xCCCCDDDD");
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
  std::vector<std::pair<std::string, std::string>> writes = {
      {"0", "0xAAAABBBB"}, {"1", "0x11112222"}, {"2", "0x33334444"},
      {"3", "0x55556666"}, {"4", "0x77778888"},
  };

  for (const auto& write : writes) {
    ExecuteCommand(
        MakeCommand({"./SSDDriver.exe", "W", write.first, write.second}));
  }

  ExecuteCommand(MakeCommand({"./SSDDriver.exe", "R", "2"}));
  EXPECT_EQ(ReadOutputFile(), "0x33334444");
}

TEST_F(SSDCommandTest, RandomCommandExecution_MixedOps) {
  std::vector<std::vector<std::string>> commands = {{"E", "23", "7"},
                                                    {"E", "34", "1"},
                                                    {"E", "29", "6"},
                                                    {"W", "14", "0x650DCB38"},
                                                    {"W", "37", "0xB823F008"},
                                                    {"W", "70", "0x19A98428"},
                                                    {"W", "57", "0x19A98428"},
                                                    {"W", "42", "0xB3517CC8"},
                                                    {"R", "47"},
                                                    {"E", "36", "6"},
                                                    {"R", "31"},
                                                    {"W", "63", "0x19A98428"},
                                                    {"E", "9", "7"},
                                                    {"E", "67", "5"},
                                                    {"E", "74", "4"},
                                                    {"E", "19", "7"},
                                                    {"W", "76", "0x24AE8F34"},
                                                    {"W", "41", "0x1AE96CD8"},
                                                    {"R", "9"},
                                                    {"W", "3", "0xF51E84CA"},
                                                    {"R", "54"},
                                                    {"R", "37"},
                                                    {"E", "5", "6"},
                                                    {"R", "79"},
                                                    {"W", "2", "0xB50674A5"},
                                                    {"R", "26"},
                                                    {"R", "56"},
                                                    {"R", "70"},
                                                    {"E", "62", "9"},
                                                    {"E", "10", "10"},
                                                    {"F"} };

  for (const auto& cmdArgs : commands) {
    std::vector<std::string> args = {"./SSDDriver.exe"};
    args.insert(args.end(), cmdArgs.begin(), cmdArgs.end());
    ParsedCommand cmd = MakeCommand(args);
    ExecuteCommand(cmd);

    // Read일 경우 결과 출력
    if (cmd.opCode == "R") {
      std::string result = ReadOutputFile();
      std::cout << "Read LBA " << cmd.lba << " → " << result << std::endl;
    }
  }
}

TEST_F(SSDCommandTest, Run_WriteThenRead_ReturnsWrittenValue) {
    CommandParser parser;
    FileIO fileio;
    CommandRunner runner(parser, fileio);

    const char* write_argv[] = { "prog", "W", "5", "0xDEADBEEF" };
    runner.Run(4, const_cast<char**>(write_argv));

    const char* read_argv[] = { "prog", "R", "5" };
    runner.Run(3, const_cast<char**>(read_argv));

    const char* erase_argv[] = { "prog", "E", "5", "1" };
    runner.Run(4, const_cast<char**>(erase_argv));

    EXPECT_EQ(ReadOutputFile(), "0xDEADBEEF");
}

TEST_F(SSDCommandTest, Run_WriteThenRead_ZeroVal) {
    CommandParser parser;
    FileIO fileio;
    CommandRunner runner(parser, fileio);

    const char* write_argv[] = { "prog", "W", "0", "0x00000000" };
    runner.Run(4, const_cast<char**>(write_argv));
}

TEST_F(SSDCommandTest, Run_WriteThenReadWithErrorFlag) {
    CommandParser parser;
    FileIO fileio;
    CommandRunner runner(parser, fileio);

    const char* write_argv[] = { "prog", "W", "-1", "0x00000000" };
    runner.Run(4, const_cast<char**>(write_argv));
}