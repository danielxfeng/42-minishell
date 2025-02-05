/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 20:56:32 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/05 15:45:12 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <fcntl.h>
#include <unistd.h>

// A helper function to deal with the get next line.
static void	here_doc_read_line(t_ast *ast, t_red_prop *prop, int *pipe_fds)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", ast->fd_out);
		line = get_next_line(ast->fd_in);
		if (!line)
		{
			close(pipe_fds[0]);
			close(pipe_fds[1]);
			exit_prog(&ast, "get_next_line()", MALLOC_ERR, EXIT_FAILURE);
		}
		if ((ft_strncmp(prop->file_name, line, ft_strlen(prop->file_name)) == 0)
			&& line[ft_strlen(prop->file_name)] == '\n')
		{
			free(line);
			break ;
		}
		write(pipe_fds[1], line, ft_strlen(line));
		free(line);
	}
}

// Helper function to handle the special case `here_doc`.
// `here_doc` uses `stdin` for input, but since `stdin` is read-only,
// we must apply a `pipe` to simulate an input stream.
//
// 1. `get_next_line` from `stdin`.
// 2. Write each line into the `pipe`.
// 3. Close the write end of the pipe to signal `EOF`.
// 4. Redirect the read end of the pipe to `stdin`.
//
// Note:
// - A pipe initially has exactly 1 read (fds[0]) and 1 write (fds[1]) endpoint.
// - When creating sub-processes via `fork`, the pipe endpoints are DUPLICATED.
//   So we have to close 2.
// - Closing the write end of the pipe signals `EOF` to the reader.
// - Once reading is complete, the read must also be closed to free resources.
static int	here_doc_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_red_prop	*prop;
	int			pipe_fds[2];

	if (pipe(pipe_fds) < 0)
		exit_prog(&ast, "pipe()", PIPE_ERR, EXIT_FAILURE);
	prop = (t_red_prop *)ast_node->prop;
	here_doc_read_line(ast, prop, pipe_fds);
	close(pipe_fds[1]);
	if (dup2(pipe_fds[0], STDIN_FILENO) < 0)
	{
		close(pipe_fds[0]);
		exit_prog(&ast, "dup2()", DUP_ERR, EXIT_FAILURE);
	}
	close(pipe_fds[0]);
	return (ast_node->right->node_handler(ast, ast_node->right));
}

// Helper function to open the file, and deal with the error handling.
static int	open_file_helper(t_ast *ast, t_red_prop *prop, bool is_in)
{
	int	access_code;
	int	open_code;
	char *file_name;

	access_code = W_OK;
	if (is_in)
		access_code = R_OK;
	open_code = O_RDONLY;
	if (!is_in)
	{
		if (prop->is_single)
			open_code = O_WRONLY | O_CREAT | O_TRUNC;
		else
			open_code = O_WRONLY | O_CREAT | O_APPEND;
	}
	file_name = ast->tokens[prop->idx];
	if (access(file_name, F_OK) == 0 && access(file_name, access_code) == -1)
		exit_prog(&ast, prop->file_name, PERMISSION_ERR, EXIT_FAILURE);
	prop->fd = open(file_name, open_code, 0644);
	if (prop->fd < 0)
		exit_prog(&ast, file_name, FILE_ERR, EXIT_FAILURE);
}

// @brief the executor of a `red` node.
//
// The `red` node has 1 child or 2 children.
//
// @param ast: the pointer to the ast tree.
// @param ast_node: the `red` node.
int	red_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_red_prop	*prop;
	int			std;
	int			res;

	prop = (t_red_prop *)ast_node->prop;
	std = STD_OUT;
	if ((prop->is_in))
	{
		if (!(prop->is_single))
			return (here_doc_handler(ast, ast_node));
		std = STD_IN;
		res = open_file_helper(ast, prop, true);
	}
	else
		res = open_file_helper(ast, prop, false);
	if (res)
		
	if (dup2(prop->fd, std) < 0)
		exit_with_err(&ast, 1, "dup2()");
	close(prop->fd);
	prop->fd = -1;
	if (ast_node->left)
		res = ast_node->left->node_handler(ast, ast_node->left);
	if (ast_node->right)
		res = ast_node->right->node_handler(ast, ast_node->right);
	return (res);
}
