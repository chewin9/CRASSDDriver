#include "gmock/gmock.h"
#include "iprocess_executor.h"
#include "shell_write.h"
#include "test.h"

#include <memory>


TEST(shell_write, ssd_write) {
	// TC 시나리오 
	// shell이 입력되어 사용자 입력'write'을 받으면, 
	// ssd app을 실행시키고 << mock 대상임
	// 저장 명령을 수행한다. 
	// 사용자 입력 예시는 "write 3 0xAAAABBBB" 
	// 실제로는 아래처럼 내려감 >>"ssd.exe W 3 0xAAAABBBB"

	// 매개 변수가 유효한지 검사를 수행한다. 

	// 없는 명령어의 경우 invalid command를 콘솔에 출력한다. 

	MockProcessExecutor executor;
	ShellWrite shell_write(&executor);

	EXPECT_CALL(executor, Process);

	shell_write.IssueWrite("write 3 0xAAAABBBB");
}
