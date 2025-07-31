#pragma once
#include <string>


class Logger {
public:
	Logger() : bUseConsolePrint{ true } {}
	void print(const std::string& function_str, const std::string& message);
	void disable_console_print(void);
private:
	bool bUseConsolePrint;
	void write_to_file(const std::string& str);
	const std::string filename = "latest.log";
};