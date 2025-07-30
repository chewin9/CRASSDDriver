#include <string>
#pragma once
#include "ICommand.h"
#include "file_io.h"
class ReadCommand : public ICommand {
  ParsedCommand& cmdInfo;
  FileIO& fileio;

 public:
	 ReadCommand(ParsedCommand& cmdInfo, FileIO& fileio) : cmdInfo{ cmdInfo }, fileio{fileio} {}
  bool Execute() override;
};