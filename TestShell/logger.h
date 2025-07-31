#pragma once
#include <string>


class Logger {
public:
	Logger() : bUseConsolePrint{ true } {}
	void print(const std::string& function_str, const std::string& message);
	void disable_console_print(void);

private:
	void write_to_file(const std::string& str);
	void print_to_console(const std::string& oss);
	std::string getCurrentTimeString(void);

	const std::string filename = "latest.log";
	bool bUseConsolePrint;
};