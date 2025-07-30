#include "file_io.h"
#include <iostream>


//bool FileIO::Write(int LBA, const std::string& value) {
//    if (!fs_.is_open()) {
//        fs_.open(SSD_NAND_FILE, std::ios::out | std::ios::app);
//        if (!fs_) return false;
//    }
//
//    fs_ << LBA << " " << value << std::endl;
//    fs_.flush();
//    return true;
//}

void FileIO::Close() {
    if (fs_.is_open()) {
        fs_.close();
    }
}

FileIO::~FileIO() {
    Close();
}