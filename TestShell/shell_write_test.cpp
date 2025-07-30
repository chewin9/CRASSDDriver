#include "gmock/gmock.h"
#include "iprocess_executor.h"
#include "shell_write.h"
#include "test.h"

#include <memory>


TEST(shell_write, ssd_write) {
	// TC �ó����� 
	// shell�� �ԷµǾ� ����� �Է�'write'�� ������, 
	// ssd app�� �����Ű�� << mock �����
	// ���� ����� �����Ѵ�. 
	// ����� �Է� ���ô� "write 3 0xAAAABBBB" 
	// �����δ� �Ʒ�ó�� ������ >>"ssd.exe W 3 0xAAAABBBB"

	// �Ű� ������ ��ȿ���� �˻縦 �����Ѵ�. 

	// ���� ��ɾ��� ��� invalid command�� �ֿܼ� ����Ѵ�. 

	MockProcessExecutor executor;
	ShellWrite shell_write(&executor);

	EXPECT_CALL(executor, Process);

	shell_write.IssueWrite("write 3 0xAAAABBBB");
}
