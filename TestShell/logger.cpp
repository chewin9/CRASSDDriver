#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
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

bool Logger::is_file_over_10k(const std::string& file) {
    // file size 
    std::ifstream in(file, std::ios::binary | std::ios::ate);
    std::streamsize size = in.tellg();
    in.seekg(0);
    if (size > 10 * 1024) return true;
    else return false;
}

std::string Logger::get_saved_log_file_name(void)
{
    std::string log_file_name = "until_" + getCurrentTimeString() + ".log";
    std::replace(log_file_name.begin(), log_file_name.end(), ':', '_');
    return log_file_name;
}

void Logger::move_file_to_log(const std::string& file)
{
    std::string log_file_name = get_saved_log_file_name();

    std::ifstream in(file, std::ios::binary | std::ios::ate);
    in.clear();
    in.seekg(0);

    std::ofstream out(log_file_name, std::ios::binary);
    if (!out) {
        std::cerr << "Cannot open or create destination file: " << log_file_name << std::endl;
        // handle error, e.g., return false or exit
    }
    out << in.rdbuf(); // Copy all contents
}

void Logger::write_to_file(const std::string& str) {

    std::ofstream file(filename, std::ios::app); // Append mode
    if (file.is_open()) {
        file << str << std::endl; // Always writes to a new line
    }
    file.close();

    if (true == is_file_over_10k(filename))
    {
        // check if other .log file exist, change file name to .zip

        // move file to YYMMDD_HHMMSS.log 
        move_file_to_log(filename);

        // remove the latest.log 
        remove(filename.c_str());
    }
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
