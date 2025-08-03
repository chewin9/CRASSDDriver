#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include "IFileIO.h"

class FileIO : public IFileIO {
public:
    std::unordered_map<int, std::string> LoadDataFromInput() override;
    void WriteValueToOutputFile(const std::string& str) override;
    void SaveData(const std::unordered_map<int, std::string>& entries) override;
    void InitBufferDir() override;
    void EraseBufferDir() override;
    void ChangeFileName(const std::vector<std::string>& in_command) override;
    std::vector<std::string> getCommandBuffer() override;

private:
    std::vector<std::string> getFileNames();
};
