#include "readcommand.h"
#include "file_io.h"
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "command_parser.h"
using namespace testing;

class ReadCommandFixture : public Test {
 public:

  const int MAX_LBA = 100;
  const std::string NO_NEED_VALUE = "";
  const bool ENABLE_ERROR_FLAG = true;
  std::string normalValue = "0xFFFFFFFF";
  FileIO fileio;
};

TEST_F(ReadCommandFixture, NormalRead) {
  for (int normalLba = 0; normalLba < MAX_LBA; normalLba++) {
    ParsedCommand cmdInfo = {"R", normalLba};
    CommandBuffer cmdbuffer{nullptr};
    SsdOperationHandler opHandler(fileio, cmdbuffer);
    ReadCommand rc(opHandler);
    rc.Execute(cmdInfo);
  }
}

TEST_F(ReadCommandFixture, AbNormalRead) {
  for (int normalLba = 0; normalLba < MAX_LBA; normalLba++) {
    ParsedCommand cmdInfo = {"R", normalLba, NO_NEED_VALUE, ENABLE_ERROR_FLAG};
    CommandBuffer cmdbuffer{nullptr};
    SsdOperationHandler opHandler(fileio, cmdbuffer);
    ReadCommand rc(opHandler);
    rc.Execute(cmdInfo);
  }
}
