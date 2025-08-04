#include "command_creator.h"
#include "command_factory.h"

struct CommandRegistrar {
  CommandRegistrar() {
    CommandFactory::registerCommand("R", CreateReadCommand);
    CommandFactory::registerCommand("W", CreateWriteCommand);
    CommandFactory::registerCommand("E", CreateEraseCommand);
    CommandFactory::registerCommand("F", CreateFlushCommand);
  }
};

static CommandRegistrar registrarInstance;