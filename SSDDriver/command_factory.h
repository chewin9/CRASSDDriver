#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "icommand.h"
#include "ssd_operation_handler.h"

class CommandFactory {
 public:
  using Creator = ICommand* (*)(SsdOperationHandler&);

  static ICommand* create(const std::string& opCode,
                          SsdOperationHandler& handler);
  static void registerCommand(const std::string& opCode, Creator creator);

 private:
  static std::unordered_map<std::string, Creator>& getRegistry();
};
