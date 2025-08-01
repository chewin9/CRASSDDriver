#pragma once

#define PRINT(log_string) m_plogger->print(__func__, m_name + log_string)
#define PRINT_NO_NAME(log_string) m_plogger->print(__func__, log_string)