#include "../src/include/mini_env.h"
#include "unity.h"
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// valgrind --leak-check=full --show-leak-kinds=all ./build/env_test

// Setup function called before each test
void	setUp(void)
{
	// Initialize or allocate resources if needed (optional)
}

// Teardown function called after each test
void	tearDown(void)
{
	// Clean up resources if needed (optional)
}

void    testCreateEnv(void)
{
    char *envp[] = {"HOME=ACT"}
}

// Main function to run the tests
int	main(void)
{
	UNITY_BEGIN();
    //RUN_TEST(testBuildTree_OnePipe);
	//RUN_TEST(testBuildTree_OneRED);
	//RUN_TEST(testBuildTree_OneCMD);
	//RUN_TEST(testBuildTree_ThreeNodes);
	//RUN_TEST(testBuildTree_MultiNodes);
	
	// We need to observe the output ourself now.

	//RUN_TEST(testExec_OneCmd);
	//RUN_TEST(testExec_CmdRed);
	//RUN_TEST(testExec_CmdRedPipe);
	//RUN_TEST(testExec_MultiRedPipe);
	//RUN_TEST(testExec_NonExistCmdRelative);
	//RUN_TEST(testExec_NonExistCmd);
	//RUN_TEST(testExec_NoReadCmd);
	//RUN_TEST(testExec_NoWriteCmd);
	//RUN_TEST(testExec_EmptyCmd);
	//RUN_TEST(testExec_Dir);
	//RUN_TEST(testExec_NotProgram);
	//RUN_TEST(testExec_MultiPipe);
	//RUN_TEST(testExec_MultiRed);
	//RUN_TEST(testExec_MultiNotExistRed);
	//RUN_TEST(testExec_MultiNotExistRedWithPipe);
	//RUN_TEST(testExec_RedNotExist);
	//RUN_TEST(testExec_RedNotRead);
	//RUN_TEST(testExec_RedDir);
	//RUN_TEST(testExec_MultiComplexRed);
	//RUN_TEST(testExec_MultiComplexRedWithPipe);
	//RUN_TEST(testExec_Heredoc);
	//RUN_TEST(testExec_MultiHeredoc);
	RUN_TEST(testExec_MultiHeredocWithPipe);
	return (UNITY_END());
}
