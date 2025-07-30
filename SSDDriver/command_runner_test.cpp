#include "command_parser.h"
#include "gmock/gmock.h"
#include "readcommand.h"
class MockReadCommand {
 public:
  MOCK_METHOD(bool, Execute, ParsedCommand cmd, (override));
};

TEST(CommandRunnerTest, ExecuteCommand) {
  CommandRunner runner;
  MockReadCommand mockRead;
  EXPECT_CALL(mockRead, Execute).Times(1);
  runner.ExecuteCommand();
}