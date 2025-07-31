#pragma once
#include "shell_write.h"
#include <memory>

#include <string>
#include <sstream>
#include <iostream>

bool ShellWrite::Run(const std::string& input) {

    if (false == checkParameterValid(input)) {
        printError();
        return false;
    }

    std::string cmdLine = "SSDDriver.exe W " + input.substr(6);
    executor_->Process(cmdLine);
    std::cout << "[Write] Done" << std::endl;
    return true;
}


bool ShellWrite::checkParameterValid(const std::string& input)
{
    // check input parameter valid
    //"write 3 0xAAAABBBB"   
    vector<string> cmdLineVector = splitBySpace(input);

    if (cmdLineVector.size() != 3) {
        return false;
    }

    if (cmdLineVector[0] != "write") {
        return false; // not expected command
    }

    int cmdLba = 0;
    if (false == convertStoI(cmdLineVector[1], cmdLba)) {
        return false;
    }

    if (!((START_LBA <= cmdLba) && (cmdLba < END_LBA))) {
        return false;  // not valid command 
    }

    string data = cmdLineVector[2];
    if (data.length() != 10) {
        return false;
    }

    if (data.find("0x") != 0) {
        return false;
    }

    if (false == is_all_uppercase(data.substr(2, 8))) {
        return false;
    }

    if (false == is_valid_unsigned(data)) {
        return false;
    }

    return true;
}

void Write::printError() {
    std::cout << "INVALID COMMAND" << std::endl;
}

std::vector<std::string> Write::splitBySpace(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string word;
    while (iss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

bool Write::convertStoI(const std::string& str, int& val) {
    size_t idx = 0;
    try {
        val = std::stoi(str, &idx);
        return idx == str.size();
    }
    catch (const std::invalid_argument&) {
        return false; // Not a number at all
    }
    catch (const::std::out_of_range&) {
        return false;
    }
}
bool Write::is_all_uppercase(const std::string& str) {

    for (char ch : str) {
        if ('A' <= ch && ch <= 'F') continue;
        if ('0' <= ch && ch <= '9') continue;
        return false;
    }
    return true;
}

bool Write::is_valid_unsigned(const std::string& str) {
    size_t idx = 0;
    try {
        std::stoul(str, &idx, 0); // base 0: auto-detect base (hex, dec, oct)
        return idx == str.size(); // true if entire string was used
    }
    catch (const std::invalid_argument&) {
        return false; // Not a number at all
    }
    catch (const std::out_of_range&) {
        return false; // Number outside unsigned long range
    }
}
