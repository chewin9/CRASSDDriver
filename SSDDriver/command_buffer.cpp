#include "command_buffer.h"

#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

void CommandBuffer::SelectOptimizer(const ParsedCommand& cmdInfo) {
    if (cmdInfo.opCode == "W") {
        optimizer = std::make_unique<WriteCommandOptimizer>();
    }
    else if (cmdInfo.opCode == "E") {
        optimizer = std::make_unique<EraseCommandOptimizer>();
    }
}

void CommandBuffer::OptimizeBuffer(const ParsedCommand& cmdInfo) {
    ParsedCommand convertedCmd = cmdInfo;
    SelectOptimizer(cmdInfo);
    if (optimizer) {
        ConvertWriteZeroValToErase(convertedCmd);
        optimizer->Optimize(cmdInfo, bufferList);
    }
}

void CommandBuffer::InitializeBuffer(const vector<string>& currentBuffer) {
  bufferList.clear();
  bufferList = ParsingStringtoBuf(currentBuffer);
}
vector<string> CommandBuffer::RegisterBuffer(
    const ParsedCommand &cmdInfo, const vector<string> &currentBuffer) {
  InitializeBuffer(currentBuffer);
  OptimizeBuffer(cmdInfo);
  return ParsingBuftoString(bufferList);
}

void CommandBuffer::ConvertWriteZeroValToErase(ParsedCommand &cmdInfo) {
  if (cmdInfo.opCode == WRITE_OPCODE && cmdInfo.value == ERASED_VALUE) {
    cmdInfo.opCode = ERASE_OPCODE;
    cmdInfo.erase_size = 1;
    cmdInfo.value = "";
  }
}

bool CommandBuffer::IsFlushNeeded(const vector<string> &currentBuffer) {
  if (currentBuffer.size() >= 5) return true;
  return false;
}

std::list<ParsedCommand> CommandBuffer::GetCommandBuffer(
    const vector<string> &currentBuffer) {
  std::list<ParsedCommand> bufferList = ParsingStringtoBuf(currentBuffer);
  return bufferList;
}

std::string CommandBuffer::ReadBuffer(const ParsedCommand &cmdInfo,
                                      const vector<string> &currentBuffer) {
  std::list<ParsedCommand> bufferList = ParsingStringtoBuf(currentBuffer);

  int readLba = cmdInfo.lba;
  for (auto cmd = bufferList.rbegin(); cmd != bufferList.rend(); ++cmd) {
    if (cmd->opCode == WRITE_OPCODE) {
      if (readLba == cmd->lba) {
        return cmd->value;
      }
    }

    if (readLba >= cmd->lba && readLba <= cmd->lba + cmd->erase_size - 1) {
      return ERASED_VALUE;
    }
  }

  return VALUE_NOT_FIND;
}

std::list<ParsedCommand> CommandBuffer::ParsingStringtoBuf(
    const std::vector<std::string> &bufferArr) {
  std::list<ParsedCommand> parsedList;

  for (const auto &bufferStr : bufferArr) {
    ParsedCommand cmd;
    std::istringstream ss(bufferStr);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, '_')) {
      tokens.push_back(token);
    }

    if (tokens.size() != 4) {
      cmd.errorFlag = true;
      continue;
    }

    cmd.opCode = tokens[1];
    cmd.lba = std::stoi(tokens[2]);

    if (cmd.opCode == WRITE_OPCODE) {
      cmd.value = tokens[3];
    } else if (cmd.opCode == ERASE_OPCODE) {
      cmd.erase_size = std::stoi(tokens[3]);
    } else {
      cmd.errorFlag = true;
    }

    parsedList.push_back(cmd);
  }

  return parsedList;
}

std::vector<std::string> CommandBuffer::ParsingBuftoString(
    const std::list<ParsedCommand> &cmdList) {
  std::vector<std::string> result;
  int index = 1;

  for (const auto &cmd : cmdList) {
    std::ostringstream oss;
    oss << index << "_" << cmd.opCode << "_" << cmd.lba << "_";

    if (cmd.opCode == WRITE_OPCODE) {
      oss << cmd.value;
    } else if (cmd.opCode == ERASE_OPCODE) {
      oss << cmd.erase_size;
    } else {
      oss << "INVALID";
    }

    result.push_back(oss.str());
    ++index;
  }

  return result;
}