#pragma once

#include "ICommand.h"
#include "command_parser.h"

class WriteCommand : public ICommand {
  ParsedCommand cmdInfo;

 public:
  WriteCommand(ParsedCommand& cmdInfo) : cmdInfo{cmdInfo} {}
  bool Execute() override;
};