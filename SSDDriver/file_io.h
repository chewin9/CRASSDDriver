#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class FileIO {
public:
    static constexpr int LBA_BYTE = 4;
    static constexpr int LBA_MIN = 0;
    static constexpr int LBA_MAX = 99;

    const std::string SSD_NAND_FILE = "ssd_nand.txt";

    void Close();

    ~FileIO();

private:
    std::fstream   fs_;
};