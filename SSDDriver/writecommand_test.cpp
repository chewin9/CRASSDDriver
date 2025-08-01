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
  FileIO fileio;
};

TEST_F(WriteCommandFixture, WriteNewFile) {
  for (int i = 0; i < MAX_VAL_SIZE; i++) {
    ParsedCommand cmdInfo = {"W", normalLba, normalValue, false};
    CommandBuffer cmdbuffer{fileio};
    SsdOperationHandler opHandler(fileio, cmdbuffer);
    WriteCommand write_command(opHandler);
    EXPECT_EQ(true, write_command.Execute(cmdInfo));
  }
}

TEST_F(WriteCommandFixture, OverwirteNewLBA) {
  ParsedCommand cmdInfo = {"W", normalLba, normalValue, false};
  CommandBuffer cmdbuffer{fileio};
  SsdOperationHandler opHandler(fileio, cmdbuffer);
  WriteCommand write_command(opHandler);
  write_command.Execute(cmdInfo);

  std::string new_value = "0xAAAABBBD";
  ParsedCommand cmdInfo_new_val = {"W", normalLba, new_value, false};
  WriteCommand write_command_with_new_val(opHandler);
  write_command.Execute(cmdInfo_new_val);

  int new_lba = 3;
  new_value = "0x1298CDEF";
  ParsedCommand cmdInfo_new_lba = {"W", new_lba, new_value, false};
  WriteCommand write_command_with_new_lba(opHandler);
  EXPECT_EQ(true, write_command.Execute(cmdInfo_new_lba));
}
