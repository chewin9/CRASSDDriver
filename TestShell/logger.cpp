#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "logger.h"

void Logger::print(const std::string& classFunc, const std::string& message)
{    
    std::string timeprint = getCurrentTimeString();
  
    std::ostringstream oss;
    oss << "[" << timeprint << "] "
        << std::left << std::setw(30) << classFunc // maximum 30 space
        << " : " << message;

    print_to_console(oss.str());
    write_to_file(oss.str());
}

void Logger::write_to_file(const std::string& str) {
    std::ofstream file(filename, std::ios::app); // Append mode
    if (file.is_open()) {
        file << str << std::endl; // Always writes to a new line
    }
    // file closes automatically when going out of scope
}

void Logger::disable_console_print(void)
{
    bUseConsolePrint = false;
}

void Logger::print_to_console(const std::string& str)
{
    if (true == bUseConsolePrint) {
        std::cout << str << std::endl;
    }
}

std::string Logger::getCurrentTimeString(void)
{
    // get time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm tm;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm, &now_time);
#else
    localtime_r(&now_time, &tm);
#endif

    std::ostringstream timeoss;
    timeoss << std::put_time(&tm, "%F %T");
    return timeoss.str();
}
