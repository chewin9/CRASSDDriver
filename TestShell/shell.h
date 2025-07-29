#include <string>
#include "process_executor.h"
class Shell {
public:
	explicit Shell(ProcessExecutor* excutor);
	std::string read(int command);

private:
	std::string findOutput(const std::string &data, int index);
	ProcessExecutor* executor_;
};