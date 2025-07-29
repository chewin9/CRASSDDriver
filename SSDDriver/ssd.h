#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>

#include <vector>
#include <cstdio> 

class FileIO {
public:
    static constexpr int LBA_BYTE = 4;
    static constexpr int LBA_MIN = 0;
    static constexpr int LBA_MAX = 99;

    bool Write(const int LBA, const std::string& value) {

        if (!fs_.is_open()) {
            fs_.open(SSD_NAND_FILE, std::ios::out | std::ios::app);
            if (!fs_) return false;
        }



        fs_ << LBA << " " << value << std::endl;

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
    std::fstream   fs_;
    std::string    SSD_NAND_FILE = "ssd_nand.txt";
};
