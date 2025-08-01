#include <filesystem>
#include <string>
#include <unordered_map>
#pragma once

class FileIO {
 public:
  std::unordered_map<int, std::string> LoadDataFromInput();
  void WriteValueToOutputFile(std::string str);
  void SaveData(std::unordered_map<int, std::string> entries);
  void InitBufferDir();
  void EraseBufferDir();
  void ChangeFileName(std::vector<std::string>& in_command);
  std::vector<std::string> getCommandBuffer();
  

 private:
  std::vector<std::string> getFileNames();
  const std::string SSD_NAND_FILE = "ssd_nand.txt";
  const std::string SSD_OUTPUT_FILE = "ssd_output.txt";
  const std::string SSD_COMMAND_BUFFER_FOLDER = "buffer";
};