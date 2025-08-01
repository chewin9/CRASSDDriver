#pragma once

class TestScriptUtil
{
public:
    static TestScriptUtil& GetInstance()
    {
        static TestScriptUtil instance;
        return instance;
    }

    unsigned int GetRandomValue(void);
    void MakeRandomValue(void);
    std::string GetRandomValueToString(void);

private:
    TestScriptUtil() { MakeRandomValue(); };
    ~TestScriptUtil() = default;
    TestScriptUtil(const TestScriptUtil&) = delete;
    TestScriptUtil& operator=(const TestScriptUtil&) = delete;
    unsigned int m_random_value = 0;
};