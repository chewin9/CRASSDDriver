#pragma once
#include "icommand.h"
#include "command_parser.h"
#include "file_io.h"
#include "readcommand.h"
class CommandFactory {
 public:
  static ICommand* create(const ParsedCommand& cmd, SsdOperationHandler& opHandler);
};