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

void	testExport_Set(void)
{
	char *free_tokens[] = {"export", "aa=bb"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
    TEST_ASSERT_EQUAL_STRING("aa", tree->env->items[1].key);
    TEST_ASSERT_EQUAL_STRING("bb", tree->env->items[1].value);
	close_the_world(&tree);
    return ;
}

// Main function to run the tests
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(testExport_Set);
	return (UNITY_END());
}