
#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <vector>

#include "command_buffer.h"
#include "command_factory.h"
#include "command_parser.h"
#include "file_io.h"
#include "ssd_operation_handler.h"

class SSDCommandSequenceTest : public ::testing::Test {
 protected:
  FileIO file_io;
  CommandBuffer* cmd_buffer;
  SsdOperationHandler* op_handler;

  void SetUp() override {
    std::remove("ssd_nand.txt");
    std::remove("ssd_output.txt");
    system("rm -rf buffer && mkdir buffer");

    cmd_buffer = new CommandBuffer(file_io);
    op_handler = new SsdOperationHandler(file_io, *cmd_buffer);
  }

  void TearDown() override {
    delete op_handler;
    delete cmd_buffer;
  }

  ParsedCommand MakeCommand(const std::vector<std::string>& args) {
    std::vector<char*> argv;
    for (auto& s : const_cast<std::vector<std::string>&>(args))
      argv.push_back(&s[0]);
    int argc = static_cast<int>(argv.size());
    CommandParser parser;
    return parser.ParseCommand(argc, argv.data());
  }

  void Execute(const std::string& opcode, const std::string& lba,
               const std::string& val = "") {
    std::vector<std::string> args = {"./SSDDriver.exe", opcode, lba};
    if (!val.empty()) args.push_back(val);
    ParsedCommand cmd = MakeCommand(args);
    ICommand* command = CommandFactory::create(cmd, *op_handler);
    ASSERT_NE(command, nullptr);
    command->Execute(cmd);
    delete command;
  }

  std::string ReadOutputFile() {
    std::ifstream in("ssd_output.txt");
    std::string line;
    std::getline(in, line);
    return line;
  }
};

TEST_F(SSDCommandSequenceTest, MixedCommandExecutionValidation) {
  // Erases
  Execute("E", "23", "7");
  Execute("E", "34", "1");
  Execute("E", "29", "6");

  // Writes
  Execute("W", "14", "0x650DCB38");
  Execute("W", "37", "0xB823F008");
  Execute("W", "70", "0x19A98428");
  Execute("W", "57", "0x19A98428");
  Execute("W", "42", "0xB3517CC8");

  Execute("R", "37");
  EXPECT_EQ(ReadOutputFile(), "0xB823F008");

  // Reads and validation
  Execute("R", "47");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("E", "36", "6");

  Execute("R", "37");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("R", "31");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("W", "63", "0x19A98428");

  Execute("R", "63");
  EXPECT_EQ(ReadOutputFile(), "0x19A98428");
  Execute("E", "9", "7");
  Execute("E", "67", "5");
  Execute("R", "63");
  EXPECT_EQ(ReadOutputFile(), "0x19A98428");
  Execute("E", "74", "4");
  Execute("E", "19", "7");
  Execute("R", "63");
  EXPECT_EQ(ReadOutputFile(), "0x19A98428");

  Execute("W", "76", "0x24AE8F34");
  Execute("W", "41", "0x1AE96CD8");

  Execute("R", "9");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("W", "3", "0xF51E84CA");

  Execute("R", "54");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("R", "37");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("E", "5", "6");

  Execute("R", "79");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("W", "2", "0xB50674A5");

  Execute("R", "26");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("R", "56");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("R", "70");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("E", "62", "9");
  Execute("R", "63");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");
  Execute("E", "10", "10");

  // Validate overwritten
  Execute("R", "14");
  EXPECT_EQ(ReadOutputFile(), "0x00000000");

  Execute("R", "2");
  EXPECT_EQ(ReadOutputFile(), "0xB50674A5");
}
