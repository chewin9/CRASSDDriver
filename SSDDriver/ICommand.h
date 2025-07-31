#include <string>
#pragma once
#include "command_parser.h"

class ICommand {
 public:
  virtual ~ICommand() = default;
  virtual bool Execute(const ParsedCommand& cmdInfo) = 0;
};
