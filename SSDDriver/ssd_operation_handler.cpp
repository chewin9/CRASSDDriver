#include "ssd_operation_handler.h"

void SsdOperationHandler::Flush() {
  std::list<ParsedCommand> bufferList = cmdBuffer.GetCommandBuffer();
  fileHandler.EraseFolder();
  fileHandler.GenFolderAndEmtyFiles();
  for (auto i : bufferList) {
    if (i.opCode == "W") {
      _Write(i);
    }

    else if (i.opCode == "E") {
      _Erase(i);
    }

  }
}

void SsdOperationHandler::Write(const ParsedCommand& cmdInfo) {
  if (cmdInfo.errorFlag) {
    fileHandler.WriteValueToOutputFile("ERROR");
    return;
  }

  if (cmdBuffer.IsFlushNeeded()) {
    Flush();
  }
  cmdBuffer.AddBuffer(cmdInfo);
  return;
}

void SsdOperationHandler::_Write(const ParsedCommand& cmdInfo) {
  nandData = fileHandler.LoadDataFromInput();
  UpdateData(cmdInfo);
  fileHandler.SaveData(nandData);
}

bool SsdOperationHandler::Read(const ParsedCommand& cmdInfo) {
  if (cmdInfo.errorFlag) {
    fileHandler.WriteValueToOutputFile("ERROR");
    return false;
  }

  std::string value = cmdBuffer.ReadBuffer(cmdInfo);
  if (value == "") {
    nandData = fileHandler.LoadDataFromInput();
    std::string value = ReadData(cmdInfo);
    fileHandler.WriteValueToOutputFile(value);
  } else {
    fileHandler.WriteValueToOutputFile(value);
  }
  return true;
}

bool SsdOperationHandler::Erase(const ParsedCommand& cmdInfo) {
  if (cmdInfo.errorFlag) {
    fileHandler.WriteValueToOutputFile("ERROR");
    return false;
  }
  if (cmdBuffer.IsFlushNeeded()) {
    Flush();
   
  }
  cmdBuffer.AddBuffer(cmdInfo);
  return true;
}

void SsdOperationHandler::_Erase(const ParsedCommand& cmdInfo) {
  nandData = fileHandler.LoadDataFromInput();

  EraseData(cmdInfo);

  fileHandler.SaveData(nandData);
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

bool SsdOperationHandler::EraseData(const ParsedCommand& cmdInfo) {
  for (int i = 0; i < cmdInfo.erase_size; i++) {
    auto it = nandData.find(cmdInfo.lba + i);
    if (it != nandData.end()) {
      nandData.erase(it);
    }
  }

  return true;
}