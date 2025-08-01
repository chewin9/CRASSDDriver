#include <string>
#pragma once
#include <unordered_map>
#include <vector>

class FileIOInterface {
public:
    virtual ~FileIOInterface() = default;

    inline static const std::string SSD_NAND_FILE = "ssd_nand.txt";
    inline static const std::string SSD_OUTPUT_FILE = "ssd_output.txt";
    inline static const std::string SSD_COMMAND_BUFFER_FOLDER = "buffer";

    virtual std::unordered_map<int, std::string> LoadDataFromInput() = 0;
    virtual void WriteValueToOutputFile(const std::string& str) = 0;
    virtual void SaveData(const std::unordered_map<int, std::string>& entries) = 0;
    virtual void InitBufferDir() = 0;
    virtual void EraseBufferDir() = 0;
    virtual void ChangeFileName(const std::vector<std::string>& in_command) = 0;
    virtual std::vector<std::string> getCommandBuffer() = 0;
};