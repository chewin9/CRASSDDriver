#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include "logger.h"

namespace fs = std::filesystem;

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

void Logger::print_to_console(const std::string& str)
{
    if (true == bUseConsolePrint) {
        std::cout << str << std::endl;
    }
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

bool Logger::is_file_over_10k(const std::string& file) {
    // file size 
    std::ifstream in(file, std::ios::binary | std::ios::ate);
    std::streamsize size = in.tellg();
    in.seekg(0);
    if (size > 10 * 1024) return true;
    else return false;
}

void Logger::move_saved_log_file(void)
{
    // check if other .log file exist,
    if (true == is_saved_log_file_exists())
    {
        std::string dir = ".";
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".log") {
                fs::path old_path = entry.path();
                if (old_path.string().find(filename) != std::string::npos) continue;
                // Construct new path: replace .log with .zip
                fs::path new_path = old_path;
                new_path.replace_extension(".zip");
                try {
                    fs::rename(old_path, new_path);
                }
                catch (const fs::filesystem_error& e) {
                    std::cerr << "Error renaming file: " << e.what() << std::endl;
                }
            }
        }
    }
}

bool file_exists(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
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

void Logger::save_last_time_printed(std::string& timeprint)
{
    lastTimePrinted = timeprint;
}

std::string Logger::get_saved_log_file_name(void)
{
    std::string log_file_name = "until_" + lastTimePrinted + ".log";
    std::replace(log_file_name.begin(), log_file_name.end(), ':', '_');
    return log_file_name;
}

void Logger::set_log_file_name(const std::string& name)
{
    filename = name;
}

void Logger::set_log_default_file_name()
{
    filename = defaultFileName;
}

bool Logger::is_saved_log_file_exists(void)
{
    bool found = false;
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.is_regular_file() && entry.path().extension() == ".log") {
            found = true;
            break; // You can stop as soon as one .log file is found
        }
    }
    return found;
}

bool Logger::is_diabled_console_print() {
    return !bUseConsolePrint;
}

void Logger::disable_console_print(void)
{
    bUseConsolePrint = false;
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
