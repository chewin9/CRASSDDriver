#include "file_io.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

std::unordered_map<int, std::string> FileIO::LoadDataFromInput() {
  std::ifstream input(SSD_NAND_FILE);
  if (!input.is_open()) {
    return {};
  }
  std::unordered_map<int, std::string> entries;
  std::string line;
  while (std::getline(input, line)) {
    std::istringstream iss(line);
    int existingLBA;
    std::string existingValue;
    if (iss >> existingLBA >> existingValue) {
      entries[existingLBA] = std::move(existingValue);
    }
  }
  return entries;
}

void FileIO::SaveData(std::unordered_map<int, std::string> entries) {
  std::ofstream output(SSD_NAND_FILE, std::ios::out | std::ios::trunc);
  if (!output.is_open()) {
    return;
  }
  for (const auto& entry : entries) {
    output << entry.first << " " << entry.second << "\n";
  }
}

void FileIO::WriteValueToOutputFile(std::string val) {
  std::ofstream outFile(SSD_OUTPUT_FILE);
  if (!outFile.is_open()) {
    return;
  }
  outFile << val << std::endl;
}

void FileIO::GenFolderAndEmtyFiles() {
  if (fs::exists(SSD_COMMAND_BUFFER_FOLDER)) return;

  fs::create_directory(SSD_COMMAND_BUFFER_FOLDER);
  fs::path dir{SSD_COMMAND_BUFFER_FOLDER};

  for (int i = 1; i <= 5; ++i) {
    fs::path filePath = dir / (std::to_string(i) + "_empty");
    std::ofstream ofs(filePath);
  }
}

void FileIO::EraseFolder() {
  fs::path dir{SSD_COMMAND_BUFFER_FOLDER};
  std::error_code ec;

  if (fs::exists(dir, ec) && fs::is_directory(dir, ec)) {
    fs::remove_all(dir, ec);
  }
}

std::vector<std::string> FileIO::LoadCommandBuffer() {
  GenFolderAndEmtyFiles();

  fs::path dir{SSD_COMMAND_BUFFER_FOLDER};

  std::vector<std::string> filenames;
  std::error_code ec;

  if (!fs::exists(dir, ec) || !fs::is_directory(dir, ec)) {
    return {};
  }

  for (const auto& command : fs::directory_iterator(dir, ec)) {
    if (ec) break;

    filenames.push_back(command.path().filename().string());
  }

  std::sort(filenames.begin(), filenames.end());
  return filenames;
}

std::vector<std::string> FileIO::LoadCommandBufferOnly() {
  GenFolderAndEmtyFiles();
  fs::path dir{SSD_COMMAND_BUFFER_FOLDER};

  std::vector<std::string> filenames;
  std::error_code ec;

  if (!fs::exists(dir, ec) || !fs::is_directory(dir, ec)) {
    return {};
  }

  for (const auto& command : fs::directory_iterator(dir, ec)) {
    if (ec) break;

    auto name = command.path().filename().string();
    if (name.find("_empty") == std::string::npos) {
      filenames.push_back(std::move(name));
    }
  }

  std::sort(filenames.begin(), filenames.end());
  return filenames;
}

void FileIO::ChangeFileName(std::vector<std::string>& in_command) {
  fs::path dir{SSD_COMMAND_BUFFER_FOLDER};
  EraseFolder();
  GenFolderAndEmtyFiles();
  auto oldNames = LoadCommandBuffer();
  std::error_code ec;

  size_t count = std::min(oldNames.size(), in_command.size());
  for (size_t i = 0; i < count; ++i) {
    fs::path oldPath = dir / oldNames[i];
    fs::path newPath = dir / in_command[i];
    fs::rename(oldPath, newPath, ec);
    if (ec) {
      throw std::runtime_error("fail to change file name");
    }
  }
}