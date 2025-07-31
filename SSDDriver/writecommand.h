#pragma once

#include "ICommand.h"
#include "ssd_operation_handler.h"

class WriteCommand : public ICommand {
  SsdOperationHandler& opHandler;

 public:
  WriteCommand(SsdOperationHandler& opHandler)
      : opHandler{opHandler} {}
  bool Execute(const ParsedCommand& cmdInfo) override;
};