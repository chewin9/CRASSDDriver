#include "shell.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

Shell::Shell(IProcessExecutor* executor) : executor_(executor) {}

std::string Shell::read(int index) {
	return shellReader_.read(index);
}
