#pragma once

#include "ICommand.h"
#include "file_io.h"

class WriteCommand : public ICommand {
  ParsedCommand cmdInfo;
  FileIO file_io;

 public:
  WriteCommand(ParsedCommand& cmdInfo) : cmdInfo{cmdInfo} {}
  bool Execute() override;
};