#include <string>
#include "process_executor.h"
class Shell {
public:
	explicit Shell(ProcessExecutor* executor);
	std::string read(int index);

private:
	std::string findOutput(const std::string &data, int index);
	ProcessExecutor* executor_;
};