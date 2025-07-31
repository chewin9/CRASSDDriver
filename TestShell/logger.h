#pragma once
#include <string>


class Logger {
public:
	Logger() : bUseConsolePrint{ true } {}
	void print(const std::string& function_str, const std::string& message);
	void disable_console_print(void);

	virtual bool is_file_over_10k(const std::string& file);
	virtual void move_file_to_log(const std::string& file);
	std::string get_saved_log_file_name(void);

private:
	void write_to_file(const std::string& str);
	void print_to_console(const std::string& oss);
	std::string getCurrentTimeString(void);

	const std::string filename = "latest.log";
	bool bUseConsolePrint;
};