#include "command_factory.h"

std::unordered_map<std::string, CommandFactory::Creator>&
CommandFactory::getRegistry() {
  static std::unordered_map<std::string, Creator> registry;
  return registry;
}

void CommandFactory::registerCommand(const std::string& opCode,
                                     Creator creator) {
  getRegistry()[opCode] = creator;
}

std::unique_ptr<ICommand> CommandFactory::create(const std::string& opCode, SsdOperationHandler& handler) {
    auto& registry = getRegistry();
    auto it = registry.find(opCode);
    if (it != registry.end()) {
        return it->second(handler);  // Factory returns unique_ptr
    }
    return nullptr;
}