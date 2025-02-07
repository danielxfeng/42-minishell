/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:16:18 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/07 10:23:28 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

// @brief to run in a loop for reading lines from here_doc.
//
// @param ast: the pointer to ast tree.
// @param prop: the property of node.
// @param stdout_backup: the backup of stdout.
static void	read_lines_helper(t_ast *ast, t_red_prop *prop, int stdout_backup)
{
	char	*line;
	char	*file_name;

	file_name = ast->tokens[prop->idx];
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if ((ft_strncmp(file_name, line, ft_strlen(file_name)) == 0)
			&& line[ft_strlen(file_name)] == '\n')
		{
			free(line);
			break ;
		}
		write(stdout_backup, line, ft_strlen(line));
		free(line);
	}
}

// @brief help here_doc_handler to read lines from here_doc
//
// @param ast: the pointer to ast tree.
// @param prop: the property of node.
// @param pipe_fds: the array of fds of pipe.
// @return the status code.
static int	here_doc_read_line(t_ast *ast, t_red_prop *prop, int *pipe_fds)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
		return_with_err_pipe("dup()", pipe_fds);
	if (dup2(ast->fd_in, STDIN_FILENO) < 0 || dup2(ast->fd_out,
			STDOUT_FILENO) < 0)
		return_with_err_pipe("dup2()", pipe_fds);
	read_lines_helper(ast, prop, stdout_backup);
	if (dup2(stdin_backup, STDIN_FILENO) < 0 || dup2(stdout_backup,
			STDOUT_FILENO) < 0)
		return_with_err_pipe("dup2()", pipe_fds);
}

// @brief help the `red` node to handle the heredoc.
//
// @param ast: the pointer to the tree.
// @param ast_node: the `red` node.
// @return the status code.
static int	here_doc_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_red_prop	*prop;
	int			pipe_fds[2];

	if (pipe(pipe_fds) < 0)
		exit_with_err(&ast, EXIT_FAIL, "pipe()");
	prop = (t_red_prop *)ast_node->prop;
	if (here_doc_read_line(ast, prop, pipe_fds) == EXIT_FAIL)
		return (EXIT_FAIL);
	close(pipe_fds[1]);
	if (dup2(pipe_fds[0], STDIN_FILENO) < 0)
	{
		close(pipe_fds[0]);
		exit_with_err(&ast, EXIT_FAIL, "dup2()");
	}
	close(pipe_fds[0]);
	return (EXIT_OK);
}

// @brief help the `red` node to open the file.
//
// @param ast: the pointer to the tree.
// @param prop: the property of the node.
// @return the status code.
static int	open_file_helper(t_ast *ast, t_red_prop *prop)
{
	int		access_code;
	int		open_code;
	char	*file_name;

	access_code = W_OK;
	if (prop->is_in)
		access_code = R_OK;
	open_code = O_RDONLY;
	if (!(prop->is_in))
	{
		open_code = O_WRONLY | O_CREAT | O_TRUNC;
		if (!(prop->is_single))
			open_code = O_WRONLY | O_CREAT | O_APPEND;
	}
	file_name = ast->tokens[prop->idx];
	if (access(file_name, F_OK) == -1)
		return (return_with_err(ENOENT, EXIT_FAIL, file_name));
	if (access(file_name, access_code) == -1)
		return (return_with_err(INVALID_ERR_NO, EXIT_FAIL, file_name));
	prop->fd = open(file_name, open_code, 0644);
	if (prop->fd < 0)
		return (return_with_err(INVALID_ERR_NO, EXIT_FAIL, file_name));
	return (EXIT_OK);
}

// @brief the executor of a `red` node.
//
// The `red` node has only 1 left leaf child.
//
// @param ast: the pointer to the ast tree.
// @param ast_node: the `red` node.
// @return the status code.
int	red_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_red_prop	*prop;
	int			res;

	debug_print_ast(ast, ast_node, "");
	prop = (t_red_prop *)ast_node->prop;
	if (prop->is_in && !(prop->is_single))
		res = here_doc_handler(ast, ast_node);
	else
	{
		res = open_file_helper(ast, prop);
		if (res != 0)
			return (res);
		if ((prop->is_in && dup2(prop->fd, STDIN_FILENO) < 0) || (!(prop->is_in)
				&& dup2(prop->fd, STDOUT_FILENO) < 0))
			exit_with_err(&ast, 1, "dup2()");
		close(prop->fd);
		prop->fd = -1;
	}
	if (ast_node->left)
		res = ast_node->left->node_handler(ast, ast_node->left);
	return (res);
}
