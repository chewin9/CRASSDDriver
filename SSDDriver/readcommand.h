#include <string>
#pragma once
#include "ICommand.h"
class ReadCommand : public ICommand {
  ParsedCommand cmdInfo;

 public:
  ReadCommand(ParsedCommand& cmdInfo) : cmdInfo{cmdInfo} {}
  bool Execute() override;
};