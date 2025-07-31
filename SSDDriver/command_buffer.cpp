#include "command_buffer.h"
#include <string>
#include <vector>
#include <list>
#include <sstream>

void CommandBuffer::AddBuffer(const ParsedCommand& cmdInfo) {
  std::vector<std::string> bufferArr;
	// std::vector<std::string> v = fileIo.GetBuffer(); -> 얘가 vector<string> v 형태로 준다.
  if (bufferArr.size() == 5) {
	// flush(bufferList);
  }
  std::list<ParsedCommand>bufferList = ParsingStringtoBuf(bufferArr);
  OptimizeBuffer(bufferList);
  bufferArr = ConvertParsedCommandToStringList(bufferList);
  // 요청 -> fileio에게 버퍼를 다시 저장해줘. fileio.SaveBuffer()
}
std::string CommandBuffer::ReadBuffer(const ParsedCommand& cmdInfo) {
  std::vector<std::string> bufferArr;
  // std::vector<std::string> v = fileIo.GetBuffer(); -> 얘가 vector<string> v
  // 형태로 준다.
  std::list<ParsedCommand> BufferCmdList = ParsingStringtoBuf(bufferArr);
  // for문 돌면서 
   
}

std::list<ParsedCommand> ParsingStringtoBuf(
    std::vector<std::string>& bufferList) {
  std::list<ParsedCommand> parsedList;

  for (const auto& bufferStr : bufferList) {
    ParsedCommand cmd;
    std::istringstream ss(bufferStr);
    std::string token;
    std::vector<std::string> tokens;

    // "_" 단위로 분할
    while (std::getline(ss, token, '_')) {
      tokens.push_back(token);
    }

    // 최소 4개 토큰 필요: [순번]_[W/E]_[LBA]_[value or size]
    if (tokens.size() != 4) {
      cmd.errorFlag = true;
      parsedList.push_back(cmd);
      continue;
    }

    cmd.opCode = tokens[1];
    cmd.lba = std::stoi(tokens[2]);

    if (cmd.opCode == "W") {
      cmd.value = tokens[3];
    } else if (cmd.opCode == "E") {
      cmd.erase_size = std::stoi(tokens[3]);
    } else {
      cmd.errorFlag = true;
    }

    parsedList.push_back(cmd);
  }

  return parsedList;
}

std::vector<std::string> ConvertParsedCommandToStringList(
    const std::list<ParsedCommand>& cmdList) {
  std::vector<std::string> result;
  int index = 1;

  for (const auto& cmd : cmdList) {
    std::ostringstream oss;
    oss << index << "_" << cmd.opCode << "_" << cmd.lba << "_";

    if (cmd.opCode == "W") {
      oss << cmd.value;
    } else if (cmd.opCode == "E") {
      oss << cmd.erase_size;
    } else {
      // Unknown opCode, skip or mark error
      oss << "INVALID";
    }

    result.push_back(oss.str());
    ++index;
  }

  return result;
}
