#include "writecommand.h"

#include <iostream>
#include "command_parser.h"
#include "gmock/gmock.h"

using namespace ::testing;

class WriteCommandFixture : public Test {
 public:

  int normalLba = 1;
  int abnormalLba = -1;

  const int MAX_VAL_SIZE = 100;
  std::string normalValue = "0xFFFFFFFF";
  std::vector<std::string> abnormalValue = {"0xFFF", "0x00000GGG", "FFFFFFFF",
                                            ""};
};

TEST_F(WriteCommandFixture, WriteNewFile) {
  for (int i = 0; i < MAX_VAL_SIZE; i++) {
    ParsedCommand cmdInfo = {"W", normalLba, normalValue, false};
    WriteCommand write_command(cmdInfo);
    EXPECT_EQ(true, write_command.Execute());
  }
}

TEST_F(WriteCommandFixture, OverwirteNewLBA) {
  ParsedCommand cmdInfo = {"W", normalLba, normalValue, false};
  WriteCommand write_command(cmdInfo);
  write_command.Execute();

  std::string new_value = "0xAAAABBBD";
  ParsedCommand cmdInfo_new_val = {"W", normalLba, new_value, false};
  WriteCommand write_command_with_new_val(cmdInfo_new_val);
  write_command.Execute();

  int new_lba = 3;
  new_value = "0x1298CDEF";
  ParsedCommand cmdInfo_new_lba = {"W", new_lba, new_value, false};
  WriteCommand write_command_with_new_lba(cmdInfo_new_lba);
  EXPECT_EQ(true, write_command.Execute());
}


TEST_F(WriteCommandFixture, AddNewEntryIfNotExist) {

    std::remove("ssd_output.txt");
    std::remove("ssd_nand.txt");

    ParsedCommand cmdInfo = { "W", normalLba, normalValue, false };
    WriteCommand write_command(cmdInfo);
    write_command.Execute();

    ParsedCommand cmdInfo = { "W", normalLba, normalValue, false };
    WriteCommand write_command(cmdInfo);
    write_command.Execute();


    int new_lba = 10;
    std::string new_value = "0xAAAABBBD";
    cmdInfo = { "W", new_lba, new_value, false };
    WriteCommand write_command(cmdInfo);
    write_command.Execute();


}