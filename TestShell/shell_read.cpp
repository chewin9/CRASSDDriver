#include "shell_read.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

ShellRead::ShellRead(IProcessExecutor* executor) : executor_(executor) {}

std::string ShellRead::fullRead(const std::string input) {

    std::tuple<std::string, std::string> parseCommand = parse_command(input);

    if (std::get<0>(parseCommand) == FULLREAD_COMMAND) {
        std::string cmdLine;
        std::ofstream file("temp_fullread.txt", std::ios::app);
        for (auto i = MIN_INDEX; i < MAX_INDEX; i++) {
            cmdLine = "SSDDriver.exe R " + std::to_string(i);
            executor_->Process(cmdLine);

            std::string readSSDData = getSSDData();
            file << readSSDData;
        }
        file.close();

        std::ifstream readFile("temp_fullread.txt");
        std::stringstream ret;
        ret << readFile.rdbuf();
        readFile.close();
        std::remove("temp_fullread.txt");

        return ret.str();

    }
    return ERROR_RETURN;
}

std::string ShellRead::read(const std::string input) {
    // Parse 하는 부분
    std::tuple<std::string, std::string> parseCommand = parse_command(input);
    
    //SSD 수행
    if (std::get<0>(parseCommand) == READ_COMMAND) {
        std::string cmdLine = "SSDDriver.exe R " + std::get<1>(parseCommand);
        executor_->Process(cmdLine);
    }
    //결과 값 읽기
    std::string SsdData = getSSDData();

    //전체 결과에서 index 값 찾기
    std::string ret;
    int index = std::stoi(std::get<1>(parseCommand));
    ret = getSSDDataWithIndex(SsdData, index);

    return ret;
}


std::string ShellRead::getSSDDataWithIndex(const std::string& data, int index) {
    if (isInvalidIndex(index)) {
        return ERROR_RETURN;
    }
    std::istringstream iss(data);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream line_stream(line);
        int idx;
        std::string value;
        if (line_stream >> idx >> value) {
            if (idx == index) {
                return value;
            }
        }
    }
    return NO_SSD_DATA; // 인덱스를 못 찾았을 때
}

std::tuple<std::string, std::string> ShellRead::parse_command(const std::string& input) {
    std::istringstream iss(input);
    std::string cmd;
    std::string num_str;
    iss >> cmd >> num_str;
    return { cmd, num_str };
}

std::string ShellRead::getSSDData(void) {
    
    std::ifstream file("ssd_output.txt");
    if (!file.is_open()) {
        std::cout << "open fail\n";
        return "";
    }
    std::string ret((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    return ret;
}

bool ShellRead::isInvalidIndex(int index) {
    if (index < MIN_INDEX || index >= MAX_INDEX) {
        return true;
    }
    return false;
}