/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:08:19 by Xifeng            #+#    #+#             */
/*   Updated: 2024/12/12 19:12:35 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../pipe_x.h"
#include <sys/wait.h>

// Just join the path as: path/cmd.
static char	*pipe_x_join_path(t_ast *ast, char *path, char *cmd)
{
	char	*joined;

	if (!*path)
		return (cmd);
	joined = ft_calloc((ft_strlen(path) + ft_strlen(cmd) + 2), sizeof(char));
	if (!joined)
		exit_prog(&ast, "pipe_x_join_path()", MALLOC_ERR, EXIT_FAILURE);
	ft_memcpy(joined, path, ft_strlen(path));
	joined[ft_strlen(path)] = '/';
	ft_memcpy(joined + ft_strlen(path) + 1, cmd, ft_strlen(cmd));
	return (joined);
}

// Handle the errors.
static bool	file_check(t_ast *ast, char *full_cmd, char *cmd)
{
	if (access(full_cmd, F_OK) < 0)
	{
		if (*cmd != '/' && ft_strncmp("./", cmd, 2) != 0)
			free(full_cmd);
		return (false);
	}
	if (access(full_cmd, X_OK) < 0)
	{
		free(full_cmd);
		exit_prog(&ast, cmd, PERMISSION_ERR, EXIT_EXEC_ERR);
	}
	return (true);
}

// A helper function for `parse_full_cmd`.
static bool	return_helper_pfc(t_cmd_prop *prop, char *cmd)
{
	prop->full_cmd = cmd;
	return (true);
}

// The helper function to join the `path` and `cmd`
// and check the permissions.
static bool	parse_full_cmd(t_ast *ast, t_cmd_prop *prop)
{
	int		i;
	char	*joined;

	if (prop->args[0][0] == '/' || ft_strncmp("./", prop->args[0], 2) == 0)
	{
		if (!file_check(ast, prop->args[0], prop->args[0]))
			exit_prog(&ast, prop->args[0], FILE_ERR, EXIT_CMD_ERR);
		else
			return (return_helper_pfc(prop, ft_strdup(prop->args[0])));
	}
	else
	{
		i = 0;
		while (ast->path[i])
		{
			joined = pipe_x_join_path(ast, ast->path[i++], prop->args[0]);
			if (file_check(ast, joined, prop->args[0]))
				return (return_helper_pfc(prop, joined));
		}
	}
	exit_prog(&ast, prop->args[0], CMD_ERR, EXIT_CMD_ERR);
	return (false);
}

// The handler for `cmd` nodes in the AST.
// This handler implements an `In-order Traversal`:
// 1. Execute the left child node (if any).
// 2. Execute the command in the current node:
//    2.1 Parse the command to get the executable path.
//    2.2 Create a new sub-process to execute the command.
//        There are 2 scenarios that `execve` returns -1:
//        2.2.1 The `cmd` is a folder which includes '/'.
//        2.2.2 The `cmd` is not a program.
//    2.3 In the parent process, wait for the sub-process to complete.
// 3. Execute the right child node (if any).
//
// Notes:
// - `execve` replaces the current process context with the new executable.
//   When `execve` is called, the sub-process will not return to this function.
// - This behavior is different from `pipe_handler`.
// - Because of the above reason, we have to start a new process for running
//   this `cmd`, otherwise we can do NOTHING after `execve`, but we have to
//   deal with the right child.
// - Because of the above reason, we don't have to free the copied resources
//   before return.
// - also check the explain in `pipe_handler`.
int	cmd_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_cmd_prop	*prop;
	int			status;

	prop = (t_cmd_prop *)ast_node->prop;
	if (ast_node->left)
		ast_node->left->node_handler(ast, ast_node->left);
	if (!(prop->args[0]) || !(prop->args[0][0]))
		exit_prog(&ast, prop->args[0], CMD_ERR, EXIT_CMD_ERR);
	parse_full_cmd(ast, prop);
	prop->pid = fork();
	if (prop->pid < 0)
		exit_prog(&ast, "fork()", FORK_ERR, EXIT_FAILURE);
	if (prop->pid == 0)
	{
		if (execve(prop->full_cmd, prop->args, ast->envp) < 0)
		{
			if (ft_strchr(prop->args[0], '/'))
				exit_prog(&ast, prop->full_cmd, DIRECTORY_ERR, EXIT_EXEC_ERR);
			exit_prog(&ast, prop->args[0], CMD_ERR, EXIT_CMD_ERR);
		}
	}
	waitpid(prop->pid, &status, 0);
	if (ast_node->right)
		return (ast_node->right->node_handler(ast, ast_node->right));
	return (return_process_res(status));
}
