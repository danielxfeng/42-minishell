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
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(E_PIPE, tree->root->type);
	close_the_world(&tree);
    return ;
}

void    testBuildTree_OneRED(void)
{
	char *free_tokens[] = {"<", "a"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(E_RED, tree->root->type);
	t_red_prop *prop = (t_red_prop *)tree->root->prop;
	TEST_ASSERT_TRUE(prop->is_single);
	TEST_ASSERT_TRUE(prop->is_in);
	close_the_world(&tree);
    return ;
}

void    testBuildTree_OneCMD(void)
{
	char *free_tokens[] = {"cmd1", "param1", "param2", "param3"};
	char **tokens = createTokens(free_tokens, 4);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 4, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(E_CMD, tree->root->type);
	t_cmd_prop *prop = (t_cmd_prop *)tree->root->prop;
	TEST_ASSERT_EQUAL_INT(0, prop->start);
	TEST_ASSERT_EQUAL_INT(4, prop->size);
	close_the_world(&tree);
    return ;
}

void    testBuildTree_ThreeNodes(void)
{
	char *free_tokens[] = {"cmd1", "param1", "param2", "<", "a", "|", "cmd2", ">>", "b"};
	char **tokens = createTokens(free_tokens, 9);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 9, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(E_PIPE, tree->root->type);
	TEST_ASSERT_EQUAL_INT(E_RED, tree->root->left->type);
	t_red_prop *red_prop = (t_red_prop *)tree->root->left->prop;
	TEST_ASSERT_TRUE(red_prop->is_single);
	TEST_ASSERT_TRUE(red_prop->is_in);
	TEST_ASSERT_EQUAL_INT(4, red_prop->idx);
	TEST_ASSERT_EQUAL_INT(E_RED, tree->root->right->type);
	red_prop = (t_red_prop *)tree->root->right->prop;
	TEST_ASSERT_FALSE(red_prop->is_single);
	TEST_ASSERT_FALSE(red_prop->is_in);
	TEST_ASSERT_EQUAL_INT(E_RED, tree->root->left->type);
	TEST_ASSERT_EQUAL_INT(8, red_prop->idx);
	TEST_ASSERT_EQUAL_INT(E_CMD, tree->root->left->left->type);
	t_cmd_prop *cmd_prop = (t_cmd_prop *)tree->root->left->left->prop;
	TEST_ASSERT_EQUAL_INT(0, cmd_prop->start);
	TEST_ASSERT_EQUAL_INT(3, cmd_prop->size);	
	TEST_ASSERT_EQUAL_INT(E_CMD, tree->root->right->left->type);
	cmd_prop = (t_cmd_prop *)tree->root->right->left->prop;
	TEST_ASSERT_EQUAL_INT(6, cmd_prop->start);
	TEST_ASSERT_EQUAL_INT(1, cmd_prop->size);	
	close_the_world(&tree);
    return ;
}

void    testBuildTree_MultiNodes(void)
{
	// cmd param1 < infile1 < infile2 > outfile0 | cmd2 | cmd3 > outfile1 | cmd4 > outfile2 > outfile3
	char *free_tokens[] = {"cmd1", "param1", ">", "infile1", "<", "infile2", ">", "outfile0", "|", "cmd2", "|", "cmd3", ">", "outfile1", "|", "cmd4", ">", "outfile2", ">", "outfile3"};
	char **tokens = createTokens(free_tokens, 20);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 20, env);
	t_ast_node *root = tree->root;
	TEST_ASSERT_EQUAL_INT(E_PIPE, root->type);
	t_ast_node *pipe1 = root->left;
	TEST_ASSERT_EQUAL_INT(E_PIPE, pipe1->type);
	t_ast_node *pipe2 = pipe1->left;
	TEST_ASSERT_EQUAL_INT(E_PIPE, pipe2->type);
	t_ast_node *red1 = root->right;
	TEST_ASSERT_EQUAL_INT(E_RED, red1->type);
	t_red_prop *red1prop = (t_red_prop *)red1->prop;
	TEST_ASSERT_EQUAL_INT(19, red1prop->idx);
	t_ast_node *red21 = pipe1->right;
	TEST_ASSERT_EQUAL_INT(E_RED, red21->type);
	t_red_prop *red21prop = (t_red_prop *)red21->prop;
	TEST_ASSERT_EQUAL_INT(13, red21prop->idx);
	t_ast_node *red22 = red1->left;
	TEST_ASSERT_EQUAL_INT(E_RED, red22->type);
	t_red_prop *red22prop = (t_red_prop *)red22->prop;
	TEST_ASSERT_EQUAL_INT(17, red22prop->idx);
	t_ast_node *red3 = pipe2->left;
	TEST_ASSERT_EQUAL_INT(E_RED, red3->type);
	t_red_prop *red3prop = (t_red_prop *)red3->prop;
	TEST_ASSERT_EQUAL_INT(7, red3prop->idx);
	t_ast_node *cmd31 = pipe2->right;
	TEST_ASSERT_EQUAL_INT(E_CMD, cmd31->type);
	t_cmd_prop *cmd31prop = (t_cmd_prop *)cmd31->prop;
	TEST_ASSERT_EQUAL_INT(9, cmd31prop->start);
	t_ast_node *cmd32 = red21->left;
	TEST_ASSERT_EQUAL_INT(E_CMD, cmd32->type);
	t_cmd_prop *cmd32prop = (t_cmd_prop *)cmd32->prop;
	TEST_ASSERT_EQUAL_INT(11, cmd32prop->start);
	t_ast_node *cmd33 = red22->left;
	TEST_ASSERT_EQUAL_INT(E_CMD, cmd33->type);
	t_cmd_prop *cmd33prop = (t_cmd_prop *)cmd33->prop;
	TEST_ASSERT_EQUAL_INT(15, cmd33prop->start);
	t_ast_node *red4 = red3->left;
	TEST_ASSERT_EQUAL_INT(E_RED, red4->type);
	t_red_prop *red4prop = (t_red_prop *)red4->prop;
	TEST_ASSERT_EQUAL_INT(5, red4prop->idx);
	t_ast_node *red5 = red4->left;
	TEST_ASSERT_EQUAL_INT(E_RED, red5->type);
	t_red_prop *red5prop = (t_red_prop *)red5->prop;
	TEST_ASSERT_EQUAL_INT(3, red5prop->idx);
	t_ast_node *cmd6 = red5->left;
	TEST_ASSERT_EQUAL_INT(E_CMD, cmd6->type);
	t_cmd_prop *cmd6prop = (t_cmd_prop *)cmd6->prop;
	TEST_ASSERT_EQUAL_INT(0, cmd6prop->start);
	TEST_ASSERT_EQUAL_INT(2, cmd6prop->size);
	close_the_world(&tree);
    return ;
}

// "cat ./pg/a"
void	testExec_OneCmd(void)
{
	char *free_tokens[] = {"cat", "./pg/a"};
	char **tokens = createTokens(free_tokens, 2);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 2, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(E_CMD, tree->root->type);
	t_cmd_prop *prop = (t_cmd_prop *)tree->root->prop;
	TEST_ASSERT_EQUAL_INT(0, prop->start);
	TEST_ASSERT_EQUAL_INT(2, prop->size);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

//"cat < ./pg/a > ./pg/c"
void	testExec_CmdRed(void)
{

	char *free_tokens[] = {"cat", "./pg/a", ">", "./pg/c"};
	char **tokens = createTokens(free_tokens, 4);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 4, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

//"cat < ./pg/a | cat > ./pg/c"
void	testExec_CmdRedPipe(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/a", "|", "cat", ">", "./pg/c"};
	char **tokens = createTokens(free_tokens, 7);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 7, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

//"cat < ./pg/a | > ./pg/c"
void	testExec_CmdRedWrongPipe(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/a", "|", ">", "./pg/c"};
	char **tokens = createTokens(free_tokens, 6);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 6, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testExec_MultiRedPipe(void)
{
	//"cat < ./pg/a < ./pg/b"
	char *free_tokens[] = {"cat", "<", "./pg/a", "<", "./pg/b"};
	char **tokens = createTokens(free_tokens, 5);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 5, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testExec_NonExistCmdRelative(void)
{
	char *free_tokens[] = {"./pg/nonexist"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(126, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testExec_NonExistCmd(void)
{
	char *free_tokens[] = {"noexist"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(127, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testExec_NoReadCmd(void)
{
	char *free_tokens[] = {"./pg/cannotread"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(126, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testExec_NoWriteCmd(void)
{
	char *free_tokens[] = {"./pg/cannotwrite"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(126, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testExec_Dir(void)
{
	char *free_tokens[] = {"./pg/dir"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(126, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testExec_EmptyCmd(void)
{
	char *free_tokens[] = {"   "};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(127, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

void	testExec_NotProgram(void)
{
	char *free_tokens[] = {"./pg/notprog"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// cat ./pg/a | cat | cat
void	testExec_MultiPipe(void)
{
	char *free_tokens[] = {"cat", "./pg/a", "|", "cat", "|", "cat"};
	char **tokens = createTokens(free_tokens, 6);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 6, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "cat < ./pg/a < ./pg/b"
void	testExec_MultiRed(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/a", "<", "./pg/b"};
	char **tokens = createTokens(free_tokens, 5);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 5, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "cat < ./pg/d"
void	testExec_RedNotExist(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/d"};
	char **tokens = createTokens(free_tokens, 3);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 3, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "cat < ./pg/d < ./pg/c"
void	testExec_MultiNotExistRed(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/dd", "<", "./pg/ee"};
	char **tokens = createTokens(free_tokens, 5);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 5, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "cat < ./pg/ww < ./pg/cc | < ./pg/ss"
void	testExec_MultiNotExistRedWithPipe(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/ww", "<", "./pg/cc", "|", "<", "./pg.ss"};
	char **tokens = createTokens(free_tokens, 8);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 8, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "./cat < ./pg/cannotread"
void	testExec_RedNotRead(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/cannotread"};
	char **tokens = createTokens(free_tokens, 3);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 3, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "cat < ./pg/dir"
void	testExec_RedDir(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/dir2"};
	char **tokens = createTokens(free_tokens, 3);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 3, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(1, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "./cat < ./pg/a > ./pg/c < ./pg/b > ./pg/d"
void	testExec_MultiComplexRed(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/a", ">", "./pg/c", "<", "./pg/b", ">>", "./pg/d"};
	char **tokens = createTokens(free_tokens, 9);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 9, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "./cat < ./pg/a > ./pg/c | cat < ./pg/b > ./pg/d"
void	testExec_MultiComplexRedWithPipe(void)
{
	char *free_tokens[] = {"cat", "<", "./pg/a", ">", "./pg/c", "|", "cat",  "<", "./pg/b", ">", "./pg/d"};
	char **tokens = createTokens(free_tokens, 11);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 11, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	debug_print_ast(tree, tree->root, "");
	close_the_world(&tree);
    return ;
}

// "cat << eof"
void	testExec_Heredoc(void)
{
	char *free_tokens[] = {"cat", "<<", "eof"};
	char **tokens = createTokens(free_tokens, 3);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 3, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "cat << eof << eof2"
void	testExec_MultiHeredoc(void)
{
	char *free_tokens[] = {"cat", "<<", "eof", "<<", "eof2"};
	char **tokens = createTokens(free_tokens, 5);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 5, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "cat << eof << eof2 > ./pg/c | << eof3"
void	testExec_MultiHeredocWithPipe(void)
{
	char *free_tokens[] = {"cat", "<<", "eof", "<<", "eof2", ">", "./pg/c", "|", "cat", "<<", "eof3"};
	char **tokens = createTokens(free_tokens, 11);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 11, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "ssp"
void	testExec_CmdNotInEnv(void)
{
	char *free_tokens[] = {"ssp"};
	char **tokens = createTokens(free_tokens, 1);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 1, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(127, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
}

// "prog 1 2 3"
void	testExec_CmdInEnv(void)
{
	char *free_tokens[] = {"prog", "1", "2", "3"};
	char **tokens = createTokens(free_tokens, 4);
	char *envp[] = {"PATH=/usr/bin/:/home/xifeng/42-minishell/pg", NULL};
	t_env *env = create_env(envp);
	t_ast *tree = build_tree(tokens, 4, env);

	TEST_ASSERT_NOT_NULL(tree);
	TEST_ASSERT_EQUAL_INT(EXIT_OK, tree->root->node_pre_handler(tree, tree->root));
	TEST_ASSERT_EQUAL_INT(0, tree->root->node_handler(tree, tree->root));
	close_the_world(&tree);
    return ;
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
	//RUN_TEST(testExec_CmdRedWrongPipe);
	//RUN_TEST(testExec_MultiRedPipe);
	//RUN_TEST(testExec_NonExistCmdRelative);
	//RUN_TEST(testExec_NonExistCmd);
	//RUN_TEST(testExec_NoReadCmd);
	//RUN_TEST(testExec_NoWriteCmd);
	//RUN_TEST(testExec_EmptyCmd);
	//RUN_TEST(testExec_Dir);
	//RUN_TEST(testExec_NotProgram);
	RUN_TEST(testExec_MultiPipe);
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
	//RUN_TEST(testExec_MultiHeredocWithPipe);
	//RUN_TEST(testExec_CmdNotInEnv);
	//RUN_TEST(testExec_CmdInEnv);
	return (UNITY_END());
}