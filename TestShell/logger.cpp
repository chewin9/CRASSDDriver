#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "logger.h"

#if (USING_WINCPP14)
#include <windows.h>

bool has_log_extension(const std::string& name) {
    size_t n = name.size();
    return n >= 4 && name.substr(n - 4) == ".log";
}

std::string replace_extension_with_zip(const std::string& name) {
    return name.substr(0, name.size() - 4) + ".zip";
}
#endif 

void Logger::print(const std::string& classFunc, const std::string& message)
{    
    std::string timeprint = getCurrentTimeString();
    save_last_time_printed(timeprint);

    std::ostringstream oss;
    oss << "[" << timeprint << "] "
        << std::left << std::setw(30) << classFunc // maximum 30 space
        << " : " << message;

    print_to_console(oss.str());
    write_to_file(oss.str());
}

void Logger::save_last_time_printed(std::string& timeprint)
{
    lastTimePrinted = timeprint;
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
    std::string log_file_name = "until_" + lastTimePrinted + ".log";
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
        move_saved_log_file();

        move_file_to_log(filename);

        remove(filename.c_str());
    }
}

bool Logger::is_saved_log_file_exists(void)
{
#if (USING_WINCPP14)
    // any .log file other than filename exist
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA("*.log", &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }
    else {
        return true;
    }
#endif 
}

void Logger::move_saved_log_file(void)
{
    // check if other .log file exist,
    if (true == is_saved_log_file_exists())
    {
#if (USING_WINCPP14)
        WIN32_FIND_DATAA fd;
        HANDLE hFind = FindFirstFileA("*.log", &fd);
        do {
            std::string old_name = fd.cFileName;
            if (old_name == filename) continue;
            if (has_log_extension(old_name)) {
                std::string new_name = replace_extension_with_zip(old_name);
                MoveFileA(old_name.c_str(), new_name.c_str());
            }
        } while (FindNextFileA(hFind, &fd));
        FindClose(hFind);
#endif
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
