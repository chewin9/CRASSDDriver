#include <ctime>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include "testscript_util.h"


unsigned int TestScriptUtil::GetRandomValue(void)
{
	return m_random_value;
}

void TestScriptUtil::MakeRandomValue(void)
{
	std::srand(std::time({}));
	m_random_value = rand();
}

std::string TestScriptUtil::GetRandomValueToString(void)
{
    char str[11];
    sprintf_s(str, "0x%08X", m_random_value);

    return str;
}
