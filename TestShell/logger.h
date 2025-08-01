#pragma once
#include <string>

#define USING_WINCPP14 (1)
class Logger {
public:
	Logger() : bUseConsolePrint{ true }, lastTimePrinted{ "" } {}
	void print(const std::string& function_str, const std::string& message);
	void save_last_time_printed(std::string& timeprint);
	void disable_console_print(void);
	bool is_diabled_console_print();

	virtual bool is_file_over_10k(const std::string& file);
	virtual bool is_saved_log_file_exists(void);
	std::string get_saved_log_file_name(void);

	void set_log_file_name(const std::string& name);
	void set_log_default_file_name(void);
private:
	void write_to_file(const std::string& str);
	void move_saved_log_file(void);
	void move_file_to_log(const std::string& file); 
	void print_to_console(const std::string& oss);
	std::string getCurrentTimeString(void);

	const std::string defaultFileName = "latest.log";
	std::string filename = defaultFileName;
	bool bUseConsolePrint;
	std::string lastTimePrinted;
};