#include "ICommand.h"
#include "command_factory.h"
#include "command_parser.h"
#include "gmock/gmock.h"

#if _DEBUG
int main(void) {
  ::testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}
#else
int main(int argc, char* argv[]) {
  CommandParser parser;
  ParsedCommand cmdInfo = parser.ParseCommand(argc, argv);
  ICommand* command = CommandFactory::create(cmdInfo);
  if (command == nullptr) return 0;
  command->Execute();
  delete command;
}
#endif
