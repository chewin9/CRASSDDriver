#include "erasecommand.h"

#include <gtest/gtest.h>

#include "command_parser.h"
#include "file_io.h"
#include "ssd_operation_handler.h"

using namespace ::testing;

class EraseCommandFixture : public Test {
 protected:
  FileIO fileIo;
  CommandBuffer cmdbuffer{std::move(std::make_unique<WriteCommandOptimizer>())};
  SsdOperationHandler handler{fileIo, cmdbuffer};
  EraseCommand erase_command{handler};

  int validLBA = 1;
  int invalidLBA = 999;
  int eraseSize = 1;
};

TEST_F(EraseCommandFixture, EraseExistingLba_NoErrorFlag_ReturnsTrue) {
  ParsedCommand cmdInfo{"E", validLBA, "", false, eraseSize};
  erase_command.Execute(cmdInfo);
}

TEST_F(EraseCommandFixture, EraseNonExistingLba_NoErrorFlag_ReturnsTrue) {
  ParsedCommand cmdInfo{"E", invalidLBA, "", false, eraseSize + 3};
  erase_command.Execute(cmdInfo);
}

TEST_F(EraseCommandFixture, EraseAnyLba_WithErrorFlag_ReturnsFalse) {
  ParsedCommand cmdInfo{"E", validLBA, "", true, eraseSize};
  erase_command.Execute(cmdInfo);
}
