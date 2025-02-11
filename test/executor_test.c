#include "../src/include/executor.h"
#include "unity.h"
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// valgrind --leak-check=full --show-leak-kinds=all ./build/executor_test
// valgrind -q --tool=none --track-fds=all ./build/executor_test

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

char **createTokens(char **tokens, int size)
{
    char **res = malloc(size * sizeof(char *));
    for (int i = 0; i < size; ++i)
    {
        res[i] = strdup(tokens[i]);
    }
    return res;
}

void    testBuildTree_OnePipe(void)
{
	char *free_tokens[] = {"|"};
	char **tokens = createTokens(free_tokens, 1);
	t_ast *tree = build_tree(tokens, 1);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(PIPE, tree->root->type);
	close_ast(&tree);
    return ;
}

void    testBuildTree_OneRED(void)
{
	char *free_tokens[] = {"<", "a"};
	char **tokens = createTokens(free_tokens, 2);
	t_ast *tree = build_tree(tokens, 2);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(RED, tree->root->type);
	t_red_prop *prop = (t_red_prop *)tree->root->prop;
	TEST_ASSERT_TRUE(prop->is_single);
	TEST_ASSERT_TRUE(prop->is_in);
	close_ast(&tree);
    return ;
}

void    testBuildTree_OneCMD(void)
{
	char *free_tokens[] = {"cmd1", "param1", "param2", "param3"};
	char **tokens = createTokens(free_tokens, 4);
	t_ast *tree = build_tree(tokens, 4);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(CMD, tree->root->type);
	t_cmd_prop *prop = (t_cmd_prop *)tree->root->prop;
	TEST_ASSERT_EQUAL_INT(0, prop->start);
	TEST_ASSERT_EQUAL_INT(4, prop->size);
	close_ast(&tree);
    return ;
}

void    testBuildTree_ThreeNodes(void)
{
	char *free_tokens[] = {"cmd1", "param1", "param2", "<", "a", "|", "cmd2", "<<", "b"};
	char **tokens = createTokens(free_tokens, 9);
	t_ast *tree = build_tree(tokens, 9);

	TEST_ASSERT_NOT_NULL(tree);
	close_ast(&tree);
    return ;
}

// Main function to run the tests
int	main(void)
{
	UNITY_BEGIN();
    RUN_TEST(testBuildTree_OnePipe);
	RUN_TEST(testBuildTree_OneRED);
	RUN_TEST(testBuildTree_OneCMD);
	RUN_TEST(testBuildTree_ThreeNodes);
	return (UNITY_END());
}