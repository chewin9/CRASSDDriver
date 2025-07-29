#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>

class FileIO {
public:
    static constexpr int LBA_BYTE = 4;

    static constexpr int LBA_MIN = 0;
    static constexpr int LBA_MAX = 99;

    bool isFileExist() const {
        //return std::filesystem::exists(file_name_);
    }

    // 파일에 데이터 쓰기 (mode에 따라 덮어쓰기 또는 추가 모드)
    bool Write(const std::string& data) {
        if (!fs_.is_open()) {
            fs_.open(file_name_, std::ios::out | std::ios::app);
            if (!fs_) return false;
        }
        fs_ << data;
        fs_.flush();
        return true;
    }

    void Close() {
        if (fs_.is_open()) fs_.close();
    }

    ~FileIO() {
        Close();
    }

private:
    std::string    file_name_;   
    std::fstream   fs_;
    std::string    SSD_NAND_FILE = "ssd_nand.txt";
};
