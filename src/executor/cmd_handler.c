/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:08:19 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/06 21:09:49 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

bool is_parse_free_cmd(char *cmd);
bool	is_empty_cmd(char *cmd);
char **get_path();
void free_path(char ***path);
char *ms_strjoin(t_ast *ast, char *path, char *cmd);


// @brief check if the full cmd is valid.
//
// @param ast: the pointer to the tree.
// @param prop: the property of node.
// @return if the cmd is valid.
static int	check_cmd(t_ast *ast, t_cmd_prop *prop)
{
	struct stat buf;
	
	if (access(prop->full_cmd, F_OK) < 0)
		return (return_with_err(INVALID_ERR_NO, EXIT_CMD_ERR, ast->tokens[prop->start]));
	if (access(prop->full_cmd, X_OK) < 0)
		return (return_with_err(INVALID_ERR_NO, EXIT_EXEC_ERR, ast->tokens[prop->start]));
	stat(prop->full_cmd, &buf);
	if (S_ISDIR(buf.st_mode))
		return (return_with_err(EISDIR, EXIT_EXEC_ERR, ast->tokens[prop->start]));
	return (true);
}

// @brief try to parse the full cmd from env.
// 
// @param ast: the pointer to ast tree.
// @param prop: the pointer to property of node.
// @return if there is existing file.
static bool parse_full_cmd(t_ast *ast, t_cmd_prop *prop)
{
	int i;
	char **path;
	char *full_cmd;

	i = 0;
	path = get_path();
	if (!path)
		return (false);
	while (path[i])
	{
		full_cmd = ms_strjoin(ast, path[i], ast->tokens[prop->start]);
		if (access(full_cmd, F_OK) == 0)
		{
			prop->full_cmd = full_cmd;
			free_path(&path);
			return (true);
		}
		free(full_cmd);
		++i;
	}
	free_path(&path);
	return (false);
}

// @brief to parse the full cmd and check if it is valid.
//
// @param ast: the pointer to ast.
// @param prop: the property of `cmd` node.
// @return the status.
static int parse_full_cmd_and_check(t_ast *ast, t_cmd_prop *prop)
{
	if (!(is_parse_free_cmd(ast->tokens[prop->start])) ||
	 !(parse_full_cmd(ast, prop)))
		return (return_with_err(ENOENT, EXIT_CMD_ERR, ast->tokens[prop->start]));
	return (check_cmd(ast, prop));
}

static void generate_argv(t_ast *ast, t_cmd_prop *prop)
{
	int i;

	prop->argv = ft_calloc(prop->size + 1, sizeof(char *));
	if (!(prop->argv))
		exit_with_err(&ast, EXIT_FAIL, "malloc()");
	i = 0;
	while (i < prop->size)
	{
		prop->argv[i] = ast->tokens[prop->start + i];
		++i;
	}
}

// @ brief to execute the `cmd` node.
//
// The command node may has 0 - 2 children.
//
// @param ast: the ast tree.
// @param ast_node: the `cmd` node.
// @return the status code.
int	cmd_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_cmd_prop	*prop;
	int			status;

	debug_print_ast(ast, ast_node, "");
	prop = (t_cmd_prop *)ast_node->prop;
	if (ast_node->left)
		ast_node->left->node_handler(ast, ast_node->left);
	if (is_empty_cmd(ast->tokens[prop->start]))
		return (return_with_err(ENOENT, EXIT_CMD_ERR, ast->tokens[prop->start]));
	if (is_builtin_func(ast->tokens[prop->start]))
		return (exec_builtin_func(ast->tokens, prop->start, prop->size));
	status = parse_full_cmd_and_check(ast, prop);
	if (status != 0)
		return (status);
	generate_argv(ast, prop);
	prop->pid = fork();
	if (prop->pid < 0)
		exit_with_err(&ast, EXIT_FAIL, "fork()");
	if (prop->pid == 0 && execve(prop->full_cmd, prop->argv, NULL) < 0)
		return (return_with_err(INVALID_ERR_NO, EXIT_EXEC_ERR, "execve()"));
	waitpid(prop->pid, &status, 0);
	if (ast_node->right)
		return (ast_node->right->node_handler(ast, ast_node->right));
	return (return_process_res(status));
}
