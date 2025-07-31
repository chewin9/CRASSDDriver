#include "file_io.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


bool FileIO::OpenInput() {
  m_input.open(SSD_NAND_FILE);
  return m_input.is_open();
}

void FileIO::CloseInput() {
    if (m_input.is_open()) {
        m_input.close();
    }
}

std::unordered_map<int, std::string> FileIO::LoadDataFromInput() {
    std::unordered_map<int, std::string> entries;
    std::string line;

    while (std::getline(m_input, line)) {
        std::istringstream iss(line);
        int existingLBA;
        std::string existingValue;

        if (iss >> existingLBA >> existingValue) {
            entries[existingLBA] = std::move(existingValue);
        }
    }
    return entries;
}

bool FileIO::UpdateData(std::unordered_map<int, std::string>& entries,
    const ParsedCommand& pc)
{
    auto it = entries.find(pc.lba);
    if (it != entries.end()) {
        it->second = pc.value;
        return true;
    }
    return false;
}



std::unordered_map<int, std::string> FileIO::getEntriesFromInput(ParsedCommand& pc) {
    OpenInput();
    auto entries = LoadDataFromInput();
    CloseInput();

    if (!UpdateData(entries, pc)) {
        entries.emplace(pc.lba, pc.value);
    }

    return entries;
}

bool FileIO::OpenOutput(std::string file) {
  m_output.open(file, std::ios::out | std::ios::trunc);
  return m_output.is_open();
}

void FileIO::CloseOutput() {
  if (m_output.is_open()) {
    m_output.close();
  }
}

void FileIO::WriteOutput(ParsedCommand pc) {
  if (pc.errorFlag) {
    WriteValueToOutputFile("ERROR");
    return;
  }

  std::unordered_map<int, std::string> entries = getEntriesFromInput(pc);
  // std::unordered_map<int, std::string> entries = LoadData();
  // updateData(entries, pc)
  
  OpenOutput(SSD_NAND_FILE);
  for (const auto& entry : entries) {
    m_output << entry.first << " " << entry.second << "\n";
  }
  CloseOutput();
}

void FileIO::WriteValueToOutputFile(std::string val) {
  std::ofstream outFile(SSD_OUTPUT_FILE);
  if (!outFile.is_open()) {
    return;
  }
  outFile << val << std::endl;
  outFile.close();
}

bool FileIO::ReadNandFile(ParsedCommand& pc) {
  if (pc.errorFlag) {
    WriteValueToOutputFile("ERROR");
    return false;
  }

  OpenInput();
  std::string val = "0x00000000";
  std::string line;

  while (std::getline(m_input, line)) {
    std::istringstream iss(line);
    int existing_LBA;
    std::string existing_value;

    if (iss >> existing_LBA >> existing_value) {
      if (existing_LBA == pc.lba) {
        val = existing_value;
        break;
      }
    }
  }

  CloseInput();
  WriteValueToOutputFile(val);

  return true;
}

FileIO::~FileIO() {}
