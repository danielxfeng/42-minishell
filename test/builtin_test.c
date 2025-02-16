#include "../src/include/executor.h"
#include "unity.h"
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// valgrind --leak-check=full --show-leak-kinds=all ./build/builtin_test

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

void	testExport_SetNormal(void)
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

void	testExport_SetEmptyValue(void)
{
	char *free_tokens[] = {"export", "aa="};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
    TEST_ASSERT_EQUAL_STRING("aa", tree->env->items[1].key);
    TEST_ASSERT_EQUAL_STRING("", tree->env->items[1].value);
	close_the_world(&tree);
    return ;
}

void	testExport_SetEmptyNULL(void)
{
	char *free_tokens[] = {"export", "aa"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
    TEST_ASSERT_EQUAL_STRING("aa", tree->env->items[1].key);
    TEST_ASSERT_NULL(tree->env->items[1].value);
	close_the_world(&tree);
    return ;
}

void	testExport_SetMulti(void)
{
	char *free_tokens[] = {"export", "aa" , "bb=gg"};
	char **tokens = createTokens(free_tokens, 3);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 3, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
    TEST_ASSERT_EQUAL_STRING("aa", tree->env->items[1].key);
    TEST_ASSERT_EQUAL_STRING("bb", tree->env->items[2].key);
    TEST_ASSERT_EQUAL_STRING("gg", tree->env->items[2].value);
    TEST_ASSERT_NULL(tree->env->items[1].value);
	close_the_world(&tree);
    return ;
}

void	testUnset_Normal(void)
{
	char *free_tokens[] = {"unset", "AA"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", "AA=bb" , NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
    TEST_ASSERT_EQUAL_INT(1, env->size);
    char *value;
    value = env_get(env, "AA");
    TEST_ASSERT_EQUAL_STRING("", value);
    free(value);
	close_the_world(&tree);
    return ;
}

void	testUnset_NotExist(void)
{
	char *free_tokens[] = {"unset", "AA"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
    TEST_ASSERT_EQUAL_INT(1, env->size);
    char *value;
    value = env_get(env, "AA");
    TEST_ASSERT_EQUAL_STRING("", value);
    free(value);
	close_the_world(&tree);
    return ;
}

void	testUnset_Multi(void)
{
	char *free_tokens[] = {"unset", "AA", "BB"};
	char **tokens = createTokens(free_tokens, 3);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", "AA=bb" , NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 3, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
    TEST_ASSERT_EQUAL_INT(1, env->size);
    char *value;
    value = env_get(env, "AA");
    TEST_ASSERT_EQUAL_STRING("", value);
    free(value);
    value = env_get(env, "BB");
    TEST_ASSERT_EQUAL_STRING("", value);
    free(value);
	close_the_world(&tree);
    return ;
}

void	testExportset_Output(void)
{
	char *free_tokens[] = {"export"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", "AA=bb", "BB", "CC=\"DD\"" , NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testENVtset(void)
{
	char *free_tokens[] = {"env"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", "AA=bb", "BB", "CC=\"DD\"" , NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testENVtset_InvalidOpt(void)
{
	char *free_tokens[] = {"env", "-sd"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", "AA=bb", "BB", "CC=\"DD\"" , NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(2, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testUnsetTest_InvalidOpt(void)
{
	char *free_tokens[] = {"unset", "-sd"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", "AA=bb", "BB", "CC=\"DD\"" , NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(2, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testExportTest_InvalidOpt(void)
{
	char *free_tokens[] = {"export", "-sd"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", "AA=bb", "BB", "CC=\"DD\"" , NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(2, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testEcho_Empty(void)
{
	char *free_tokens[] = {"echo"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	printf("empty line start\n");
	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
	printf("empty line end\n");
    return ;	
}

void	testEcho_Print(void)
{
	char *free_tokens[] = {"echo", "aaa", "$?", "bbb", "$?"};
	char **tokens = createTokens(free_tokens, 5);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 5, env);
	tree->prev_status = 34;

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void	testEcho_PrintWithN(void)
{
	char *free_tokens[] = {"echo", "-n", "aaa", "$?", "bbb", "$?"};
	char **tokens = createTokens(free_tokens, 6);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 6, env);
	tree->prev_status = 35;

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
	printf("!!!line end\n");
    return ;	
}

void	testEcho_WrongOpt(void)
{
	char *free_tokens[] = {"echo", "-p", "aaa", "$?", "bbb", "$?"};
	char **tokens = createTokens(free_tokens, 6);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 6, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(2, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void testPwd(void)
{
	char *free_tokens[] = {"pwd"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void testPwd_Opt(void)
{
	char *free_tokens[] = {"pwd", "-s"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(2, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void testPwd_Arguments(void)
{
	char *free_tokens[] = {"pwd", "aa"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void testCd(void)
{
	char *free_tokens[] = {"cd", "sdf"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);
	

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void testCd_Opt(void)
{
	char *free_tokens[] = {"cd", "-n"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);
	

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(2, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}


void testCd_Arguments(void)
{
	char *free_tokens[] = {"cd", "sd", "sdd"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);
	

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void testExit(void)
{
	char *free_tokens[] = {"exit", "29"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);
	

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(29, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void testExit_WrongNumber(void)
{
	char *free_tokens[] = {"exit", "29a"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);
	

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(2, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void testExit_TooManyArguments(void)
{
	char *free_tokens[] = {"exit", "2", "3"};
	char **tokens = createTokens(free_tokens, 3);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 3, env);
	

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

void testExit_Default(void)
{
	char *free_tokens[] = {"exit"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);
	

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;	
}

// Main function to run the tests
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(testExport_SetNormal);
    RUN_TEST(testExport_SetEmptyValue);
    RUN_TEST(testExport_SetEmptyNULL);
    RUN_TEST(testExport_SetMulti);
    RUN_TEST(testUnset_Normal);
    RUN_TEST(testUnset_NotExist);
    RUN_TEST(testUnset_Multi);
    // now we need to monitor the output.
    RUN_TEST(testExportset_Output);
    RUN_TEST(testENVtset);
	RUN_TEST(testENVtset_InvalidOpt);
	RUN_TEST(testUnsetTest_InvalidOpt);
	RUN_TEST(testExportTest_InvalidOpt);
	RUN_TEST(testEcho_Empty);
	RUN_TEST(testEcho_Print);
	RUN_TEST(testEcho_PrintWithN);
	RUN_TEST(testEcho_WrongOpt);
	RUN_TEST(testPwd);
	RUN_TEST(testPwd_Opt);
	RUN_TEST(testPwd_Arguments);
	RUN_TEST(testCd);
	RUN_TEST(testCd_Opt);
	RUN_TEST(testCd_Arguments);
	// To test this, should commmet the exit, also cannot test overflow.
	RUN_TEST(testExit);
	RUN_TEST(testExit_WrongNumber);
	RUN_TEST(testExit_Default);
	RUN_TEST(testExit_Default);
	RUN_TEST(testExit_TooManyArguments);
	return (UNITY_END());
}