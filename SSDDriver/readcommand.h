#include <string>
#pragma once
#include "ICommand.h"
#include "ssd_operation_handler.h"
class ReadCommand : public ICommand {
  SsdOperationHandler& opHandler;

 public:
  ReadCommand(SsdOperationHandler& opHandler)
      : opHandler{opHandler} {}
  bool Execute(const ParsedCommand& cmdInfo) override;
};