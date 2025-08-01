#pragma once

#define PRINT(log_string) Logger::getInstance().print(__func__, m_name + log_string)
#define PRINT_NO_NAME(log_string) Logger::getInstance().print(__func__, log_string)