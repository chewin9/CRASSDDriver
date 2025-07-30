#include <string>
#include "process_executor.h"
#include "shell_read.h"

class Shell {
public:
	Shell(IProcessExecutor* executor);
	std::string read(int index);

private:
	IProcessExecutor* executor_;
	ShellRead shellReader_;
};