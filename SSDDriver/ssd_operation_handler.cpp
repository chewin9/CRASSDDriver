#include "ssd_operation_handler.h"

void SsdOperationHandler::Write(const ParsedCommand& cmdInfo) {
  if (IsErrorExist(cmdInfo)) return;

  if (cmdInfo.erase_size == 0) return;

  if (cmdBuffer.IsFlushNeeded()) {
    Flush();
  }
  cmdBuffer.RegisterBuffer(cmdInfo);
  return;
}

bool SsdOperationHandler::Read(const ParsedCommand& cmdInfo) {
  if (IsErrorExist(cmdInfo)) return false;

  std::string value = cmdBuffer.ReadBuffer(cmdInfo);
  if (value == "") {
    value = ReadFromNand(cmdInfo);
  }
  fileHandler.WriteValueToOutputFile(value);
  return true;
}

bool SsdOperationHandler::Erase(const ParsedCommand& cmdInfo) {
  if (IsErrorExist(cmdInfo)) return false;
  if (cmdBuffer.IsFlushNeeded()) {
    Flush();
  }
  cmdBuffer.RegisterBuffer(cmdInfo);
  return true;
}

void SsdOperationHandler::Flush() {

  std::list<ParsedCommand> bufferList = cmdBuffer.GetCommandBuffer();
  fileHandler.EraseBufferDir();
  fileHandler.InitBufferDir();
  FlushToNand(bufferList);

}


void SsdOperationHandler::WriteToNand(const ParsedCommand& cmdInfo) {
  nandData = fileHandler.LoadDataFromInput();
  UpdateData(cmdInfo);
  fileHandler.SaveData(nandData);
}

std::string SsdOperationHandler::ReadFromNand(const ParsedCommand& cmdInfo) {
  nandData = fileHandler.LoadDataFromInput();
  std::string value = ReadData(cmdInfo);
  return value;
}

void SsdOperationHandler::EraseFromNand(const ParsedCommand& cmdInfo) {
  nandData = fileHandler.LoadDataFromInput();
  EraseData(cmdInfo);
  fileHandler.SaveData(nandData);
}

void SsdOperationHandler::FlushToNand(std::list<ParsedCommand>& bufferList) {
  for (auto cmd : bufferList) {
    if (cmd.opCode == "W") {
      WriteToNand(cmd);
    }

    else if (cmd.opCode == "E") {
      EraseFromNand(cmd);
    }
  }
}

std::string SsdOperationHandler::ReadData(const ParsedCommand& cmdInfo) {
  std::string value = "0x00000000";
  auto it = nandData.find(cmdInfo.lba);
  if (it != nandData.end()) {
    value = it->second;
  }
  return value;
}

void SsdOperationHandler::UpdateData(const ParsedCommand& cmdInfo) {
  auto it = nandData.find(cmdInfo.lba);
  if (it != nandData.end()) {
    it->second = cmdInfo.value;
    return;
  }

  nandData.emplace(cmdInfo.lba, cmdInfo.value);
  return;
}

void SsdOperationHandler::EraseData(const ParsedCommand& cmdInfo) {
  for (int i = 0; i < cmdInfo.erase_size; i++) {
    auto it = nandData.find(cmdInfo.lba + i);
    if (it != nandData.end()) {
      nandData.erase(it);
    }
  }
}
  return true;
}

bool SsdOperationHandler::IsErrorExist(const ParsedCommand& cmdInfo) {
  if (cmdInfo.errorFlag) {
    fileHandler.WriteValueToOutputFile("ERROR");
    return true;
  }
  return false;
}
