#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "shell_help.h"
#include "test.h"

TEST(ShellHelpTest, CrewIntroducePrintsCorrectOutput) {
    MockProcessExecutor mock_executor;
    ShellHelp help(&mock_executor);

    // cout redirect
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());

    help.Run("");

    std::cout.rdbuf(old_cout); // restore

    std::string expected = "ÆÀ¸í: All - pass\n"
        "ÆÀ¿ø : ÀÌ½ÂÃ¶(ÆÀÀå), Áø¿¬±â, ¹ÚÁ¤±Ù, ÀÌÁØ¼®, ±è¿¹¼Ö, °­ÀÎÇý\n"
     "How to use help command\n"
     "  -help write\n"
    "  -help read\n"
    "  -help exit\n"
    "  -help fullwrite\n"
    "  -help fullread\n";
    EXPECT_EQ(oss.str(), expected);
}
TEST(ShellHelpTest, shellhelpwrite) {
    MockProcessExecutor mock_executor;
    ShellHelp help(&mock_executor);

    // cout redirect
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());

    help.Run("help write");

    std::cout.rdbuf(old_cout); // restore


    std::string expected = "write command: write [LBA] [Value]\n"
        "  - \"write\" must be in lowercase.\n"
        "  - [LBA]: 0 ~ 100\n"
        "  - [Value]:It must always start with 0x and be written as a 10-character string.\n"
        "  - ex) write 0 0x12345678\n";
       
    EXPECT_EQ(oss.str(), expected);
}

TEST(ShellHelpTest, shellhelpread) {
    MockProcessExecutor mock_executor;
    ShellHelp help(&mock_executor);

    // cout redirect
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());

    help.Run("help read");

    std::cout.rdbuf(old_cout); // restore


    std::string expected = "read command: read [LBA]\n"
    "  - \"read\" must be in lowercase.\n"
    "  - [LBA]: 0 ~ 100\n"
    "  - ex) read 0 \n"; 
    
    EXPECT_EQ(oss.str(), expected);
}

TEST(ShellHelpTest, shellhelpexit) {
    MockProcessExecutor mock_executor;
    ShellHelp help(&mock_executor);

    // cout redirect
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());

    help.Run("help exit");

    std::cout.rdbuf(old_cout); // restore


    std::string expected = "exit command: exit\n"
    "  - Test Shell is terminated.\n"
    "  - ex) exit\n"; 
    
    EXPECT_EQ(oss.str(), expected);
}

TEST(ShellHelpTest, shellhelpfullwrite) {
    MockProcessExecutor mock_executor;
    ShellHelp help(&mock_executor);

    // cout redirect
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());

    help.Run("help fullwrite");

    std::cout.rdbuf(old_cout); // restore


    std::string expected =  "fullwrite command: fullwrite [Value]\n"
    "  - A write operation is performed on all LBA areas.\n"
    "  - ex) fullwrite 0x12345678\n";
    
    EXPECT_EQ(oss.str(), expected);
}

TEST(ShellHelpTest, shellhelpfullread) {
    MockProcessExecutor mock_executor;
    ShellHelp help(&mock_executor);

    // cout redirect
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());

    help.Run("help fullread");

    std::cout.rdbuf(old_cout); // restore


    std::string expected = "fullread command: fullread\n"
    "  - A read operation is performed from LBA 0 to 99.\n"
    "  - All values of the SSD are displayed on the screen.\n"
    "  - ex) fullread\n";
    
    EXPECT_EQ(oss.str(), expected);
}