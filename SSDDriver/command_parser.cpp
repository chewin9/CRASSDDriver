#include "command_parser.h"

#include <stdexcept>

void CommandParser::CheckException(int argc, char* argv[], const ParsedCommand& cmd) {
  if (argc < 3) {
    throw std::invalid_argument("Not Enough Arguments!");
  }
  if (argc == 3) {
    if (argv[1] == "W") throw std::invalid_argument("Not Enough Arguments!");
  }
  if (argc == 4) {
    if (argv[1] == "R") throw std::invalid_argument("Not Enough Arguments!");
  }
}

bool CommandParser::IsLbaErrorExist(const ParsedCommand& cmd) {
  if (cmd.lba < MIN_VAL || cmd.lba > MAX_VAL) {
    return true;
  }

  return false;
}

bool CommandParser::IsValueErrorExist(const ParsedCommand& cmd) {
  if (cmd.opCode == "R") return false;
  if (cmd.opCode == "E") return false;
  if (cmd.value == "" || cmd.value.size() != SIZE_OF_VALUE ||
      (cmd.value.size() > SIZE_OF_HEX_NOTATION &&
       cmd.value.substr(0, SIZE_OF_HEX_NOTATION) != "0x")) {
    return true;
  }

  for (int i = SIZE_OF_HEX_NOTATION; i < SIZE_OF_VALUE; ++i) {
    char c = cmd.value[i];
    if (!isdigit(c) && !(c >= 'A' && c <= 'F')) {
      return true;
    }
  }
  return false;
}

bool CommandParser::IsEraseSizeErrorExist(const ParsedCommand& cmd) {
    if (cmd.opCode == "E") {
        if (cmd.erase_size < MIN_VAL || cmd.erase_size >= SIZE_OF_VALUE)
            return true;

        if (cmd.lba + cmd.erase_size > 100) return true;
    }
    return false;
}

ParsedCommand CommandParser::ParseCommand(int argc, char* argv[]) {
  ParsedCommand cmd;
  CheckException(argc, argv, cmd);
  cmd.opCode = argv[1];
  cmd.lba = std::stoi(argv[2]);
  if (cmd.opCode == "W") cmd.value = argv[3];
  else if (cmd.opCode == "E") cmd.erase_size = std::stoi(argv[3]);

  if (IsLbaErrorExist(cmd) || IsValueErrorExist(cmd) || IsEraseSizeErrorExist(cmd)) {
    cmd.errorFlag = true;
  }
  return cmd;
}