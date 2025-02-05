/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 19:42:01 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/05 21:14:48 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <sys/wait.h>
#include <unistd.h>

int	return_process_res(int status);

// @brief a helper function to handle the file discriptors in sub-process
//
// @param ast: the pointer to tree.
// @param prop: the property of node.
// @param is_pipe_input: is this the input fd? or output?
static void	handle_sub_fds(t_ast *ast, t_pipe_prop *prop, bool is_pipe_input)
{
	int	src;
	int	dest;

	src = prop->fds[1];
	dest = STDOUT_FILENO;
	if (is_pipe_input)
	{
		src = prop->fds[0];
		dest = STDIN_FILENO;
	}
	if (dup2(src, dest) < 0)
		exit_with_err(&ast, EXIT_CMD_ERR, "dup2()");
	close(prop->fds[0]);
	close(prop->fds[1]);
}

// @brief a helper function to handle the sub-process for a `pipe` node.
//
// @param ast: the pointer to tree.
// @param node: the `pipe` node.
// @param prop: the property of node.
// @param direction: LEFT child or RIGHT?
static void	perform_sub_proc(t_ast *ast, t_ast_node *node, t_pipe_prop *prop,
		int direction)
{
	t_ast_node	*child;

	child = node->left;
	if (direction)
		child = node->right;
	prop->pids[direction] = fork();
	if (prop->pids[direction] < 0)
	{
		if (direction == RIGHT)
			waitpid(prop->pids[LEFT], NULL, 0);
			exit_with_err(&ast, EXIT_FAIL, "fork()");
	}
	if (prop->pids[direction] == 0)
	{
		handle_sub_fds(ast, prop, direction);
		child->node_handler(ast, child);
		exit_without_err(&ast);
	}
}

// @brief the handler of `pipe` executor.
//
// For a pipe node, it always have 2 children.
//
// @param ast: the ast tree.
// @param ast_node: the node to be executed.
// @return the exit code from right child.
int	pipe_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_pipe_prop	*prop;
	int			status;

	debug_print_ast(ast, ast_node, "");
	prop = (t_pipe_prop *)ast_node->prop;
	if (pipe(prop->fds) < 0)
		exit_with_err(&ast, 1, "pipe()");
	perform_sub_proc(ast, ast_node, prop, LEFT);
	close(prop->fds[1]);
	perform_sub_proc(ast, ast_node, prop, RIGHT);
	close(prop->fds[0]);
	waitpid(prop->pids[LEFT], NULL, 0);
	waitpid(prop->pids[RIGHT], &status, 0);
	status = return_process_res(status);
	prop->fds[1] = -1;
	prop->fds[0] = -1;
	return (status);
}
