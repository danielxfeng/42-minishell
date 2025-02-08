/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:16:18 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 21:15:11 by Xifeng           ###   ########.fr       */
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
			line = NULL;
			break ;
		}
		write(prop->fd, line, ft_strlen(line));
		free(line);
	}
}

// @brief help the `red` node to handle the heredoc.
//
// @param ast: the pointer to the tree.
// @param prop: the property of the node.
// @return the status code.
static int	here_doc_handler(t_ast *ast, t_red_prop *prop)
{
	int		status;
	int		stdin_backup;
	int		stdout_backup;
	char	*line;

	prop->fd = open("./here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (prop->fd < 0)
		exit_with_err(&ast, EXIT_FAIL, "minishell: open");
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
		return_with_err(INVALID_ERR_NO, EXIT_FAIL, "minishell: dup");
	if (dup2(ast->fd_in, STDIN_FILENO) < 0 || dup2(ast->fd_out,
			STDOUT_FILENO) < 0)
		return_with_err(INVALID_ERR_NO, EXIT_FAIL, "minishell: dup2");
	read_lines_helper(ast, prop, stdout_backup);
	if (dup2(stdin_backup, STDIN_FILENO) < 0 || dup2(stdout_backup,
			STDOUT_FILENO) < 0)
		return_with_err(INVALID_ERR_NO, EXIT_FAIL, "minishell: dup2");
	prop->is_open = true;
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
		return (return_with_err(ENOENT, EXIT_FAIL, file_name)); // todo
	if (access(file_name, access_code) == -1)
		return (return_with_err(INVALID_ERR_NO, EXIT_FAIL, file_name)); // todo
	prop->fd = open(file_name, open_code, 0644);
	if (prop->fd < 0)
		return (return_with_err(INVALID_ERR_NO, EXIT_FAIL, file_name)); // todo
	prop->is_open = true;
	return (EXIT_OK);
}

// @brief open the file for redirection and set the `is_skip` flag.
// By post-order since we need open the left node first.
//
// @param node: the current node.
// @param prop: the property of the node.
// @param is_in: the direction of the node.
// @return the status code.
static int	open_file(t_ast *ast, t_ast_node *node, t_red_prop *prop, bool is_in)
{
	int	res;

    if (!(node->left) || node->left->type != RED)
        return  (EXIT_OK);
    prop = (t_red_prop *)node->left->prop;
    if (prop->is_in == is_in)
        prop->is_skip = true;
	res = open_file(ast, node->left, prop, is_in);
	if (res != EXIT_OK)
		return (res);
	if (!prop->is_open)
	{
		if (prop->is_in && !(prop->is_single))
			res = here_doc_handler(ast, node);
		else
			res = open_file_helper(ast, prop);
		if (res != EXIT_OK)
			return (res);		
	}
	return (EXIT_OK);
}

// @brief the executor of a `red` node.
//
// The `red` node has 1 possible left child for cmd,
// and 1 possible right child for reds.
//
// The pipeline:
// 1 Perform a post-order travelsal to open the file (file or here_doc),
// and set the `is_skip` sign first.
// Because the file in left node is opened first,
// and for each direction, only right most node is redirected.
// 2 Perform the in-order travelsal to apply the redirection.
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
	open_file(ast, ast_node, prop, prop->is_in);
	if (!prop->is_skip)
	{
		if ((prop->is_in && dup2(prop->fd, STDIN_FILENO) < 0) || (!(prop->is_in) 
		&& dup2(prop->fd, STDOUT_FILENO) < 0))
			exit_with_err(&ast, EXIT_FAIL, "minishell: dup2");		
	}
	close(prop->fd);
	prop->fd = -1;
	if (prop->is_in && !(prop->is_single))
		unlink("./here_doc_tmp");
	if (ast_node->left)
		res = ast_node->left->node_handler(ast, ast_node->left);
	return (res);
}
