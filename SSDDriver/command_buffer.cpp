#include "command_buffer.h"

#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

void CommandBuffer::InitializeBuffer() {
  writeCommandList = {};
  eraseCommandList = {};
  bufferList = {};
}
void CommandBuffer::RegisterBuffer(const ParsedCommand &cmdInfo) {
  InitializeBuffer();
  std::vector<std::string> bufferArr = fileio.LoadCommandBufferOnly();
  bufferList = ParsingStringtoBuf(bufferArr);

  OptimizeBuffer(cmdInfo);
  bufferArr = ParsingBuftoString(bufferList);
  fileio.ChangeFileName(bufferArr);
}

void CommandBuffer::ConvertWriteZeroValToErase(ParsedCommand &cmdInfo) {
  if (cmdInfo.opCode == WRITE_OPCODE && cmdInfo.value == ERASED_VALUE) {
    cmdInfo.opCode = ERASE_OPCODE;
    cmdInfo.erase_size = 1;
    cmdInfo.value = "";
  }
}

void CommandBuffer::DivideWriteAndEraseBuffer() {
  for (auto i : bufferList) {
    if (i.opCode == WRITE_OPCODE)
      writeCommandList.push_back(i);
    else if (i.opCode == ERASE_OPCODE)
      eraseCommandList.push_back(i);
  }
}

void CommandBuffer::MergeWriteAndEraseBuffer(ParsedCommand cmdInfo) {
  eraseCommandList.splice(eraseCommandList.end(), writeCommandList);
  bufferList = eraseCommandList;
}

void CommandBuffer::OptimizeWriteCommand(ParsedCommand &cmdInfo) {
  for (auto it = writeCommandList.begin(); it != writeCommandList.end();) {
    if (it->lba == cmdInfo.lba) {
      it->value = cmdInfo.value;
      eraseCommandList.splice(eraseCommandList.end(), writeCommandList);
      bufferList = eraseCommandList;
      return;
    }
    ++it;
  }
  writeCommandList.push_back(cmdInfo);
}

void CommandBuffer::IgnoreWrite(int& mergedStart, int& mergedEnd) {
  for (auto it = writeCommandList.begin(); it != writeCommandList.end();) {
    int writeLba = it->lba;
    if (writeLba >= mergedStart && writeLba <= mergedEnd) {
      it = writeCommandList.erase(it);
    } else {
      ++it;
    }
  }
}

bool CommandBuffer::MergeErase(int& mergedStart, int& mergedEnd) {
  for (auto it = eraseCommandList.begin(); it != eraseCommandList.end();) {
    if (it->lba == mergedEnd - 1 || it->lba + it->erase_size == mergedStart ||
        (it->lba <= mergedStart &&
         it->lba + it->erase_size - 1 >= mergedStart) ||
        (it->lba <= mergedEnd && it->lba + it->erase_size - 1 >= mergedEnd)) {
      mergedStart = std::min(mergedStart, it->lba);
      mergedEnd = std::max(mergedEnd, it->lba + it->erase_size - 1);
      it = eraseCommandList.erase(it);
      return true;
    } else {
      ++it;
    }
  }

  return false;
}

void CommandBuffer::RearrangeMergedErase(int &mergedStart, int &mergedEnd) {
  int curStart = mergedStart;
  while (curStart <= mergedEnd) {
    int curEnd = std::min(curStart + MAX_RANGE - 1, mergedEnd);
    eraseCommandList.push_back(
        {"E", curStart, "", false, curEnd - curStart + 1});
    curStart = curEnd + 1;
  }
}

void CommandBuffer::OptimizeEraseCommand(ParsedCommand cmdInfo) {
  int mergedStart = cmdInfo.lba;
  int mergedEnd = cmdInfo.lba + cmdInfo.erase_size - 1;

  IgnoreWrite(mergedStart, mergedEnd);
  bool merged = MergeErase(mergedStart, mergedEnd);

  if (merged) {
    RearrangeMergedErase(mergedStart, mergedEnd);
  } else {
    eraseCommandList.push_back(cmdInfo);
  }
}

void CommandBuffer::OptimizeBuffer(const ParsedCommand &cmd) {
  ParsedCommand cmdInfo = cmd;
  ConvertWriteZeroValToErase(cmdInfo);
  DivideWriteAndEraseBuffer();

  if (cmdInfo.opCode == WRITE_OPCODE) {
    OptimizeWriteCommand(cmdInfo);
    MergeWriteAndEraseBuffer(cmdInfo);
    return;
  }

  OptimizeEraseCommand(cmdInfo);
  MergeWriteAndEraseBuffer(cmdInfo);

  return;
}

bool CommandBuffer::IsFlushNeeded() {
  std::vector<std::string> bufferArr = fileio.getCommandBuffer();
  if (bufferArr.size() >= 5) return true;
  return false;
}

std::list<ParsedCommand> CommandBuffer::GetCommandBuffer() {
  std::vector<std::string> bufferArr = fileio.getCommandBuffer();
  std::list<ParsedCommand> bufferList = ParsingStringtoBuf(bufferArr);
  return bufferList;
}

std::string CommandBuffer::ReadBuffer(const ParsedCommand &cmdInfo) {
  std::vector<std::string> bufferArr = fileio.getCommandBuffer();

  std::list<ParsedCommand> bufferList = ParsingStringtoBuf(bufferArr);

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
    std::vector<std::string> &bufferArr) {
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
