#include <string>
#include "process_executor.h"
class Shell {
public:
	Shell(IProcessExecutor* executor);
	std::string read(int index);

private:
	std::string findOutput(const std::string &data, int index);
	IProcessExecutor* executor_;
};