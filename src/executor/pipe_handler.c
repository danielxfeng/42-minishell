/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:16:12 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/17 17:45:14 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <sys/wait.h>
#include <unistd.h>

int			return_process_res(int status);

// @brief a helper function to handle the file discriptors in sub-process
//
// @param ast: the pointer to tree.
// @param prop: the property of node.
// @param is_pipe_input: is this the input fd? or output?
// @param fds: the fds of pipe.
static void	handle_sub_fds(t_ast *ast, t_pipe_prop *prop, bool is_pipe_input,
		int *fds)
{
	int	src;
	int	dest;

	src = fds[1];
	dest = STDOUT_FILENO;
	if (is_pipe_input)
	{
		src = fds[0];
		dest = STDIN_FILENO;
	}
	if (dup2(src, dest) < 0)
		exit_with_err(&ast, EXIT_CMD_ERR, "minishell: dup2");
	close(fds[0]);
	close(fds[1]);
}

// @brief a helper function to handle the sub-process for a `pipe` node.
//
// @param ast: the pointer to tree.
// @param node: the `pipe` node.
// @param direction: LEFT child or RIGHT?
// @param pipe_fds: the fds of pipe.
static void	perform_sub_proc(t_ast *ast, t_ast_node *node, int direction,
		int *pipe_fds)
{
	t_ast_node	*child;
	t_pipe_prop	*prop;

	child = node->left;
	if (direction)
		child = node->right;
	prop = (t_pipe_prop *)node->prop;
	prop->pids[direction] = fork();
	if (prop->pids[direction] < 0)
	{
		if (direction == RIGHT)
			waitpid(prop->pids[LEFT], NULL, 0);
		exit_with_err(&ast, EXIT_FAIL, "minishell: fork");
	}
	if (prop->pids[direction] == 0)
	{
		handle_sub_fds(ast, prop, direction, pipe_fds);
		child->node_handler(ast, child);
		exit_without_err(&ast);
	}
}

// @brief the handler of `pipe` executor.
//
// For a pipe node, it always have 2 children.
//
// We create a pipe if this is our first pipe.
// We then reuse this pipe even we have multiple pipe nodes.
// We setup 2 sub-processes to handle the left/right child.
//
// @param ast: the ast tree.
// @param ast_node: the node to be executed.
// @return the exit code from right child.
int	pipe_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_pipe_prop	*prop;
	int			status;
	int			fds[2];

	//debug_print_ast(ast, ast_node, "Exec Pipe.");
	prop = (t_pipe_prop *)ast_node->prop;
	if (!prop->is_piped && pipe(fds) < 0)
		exit_with_err(&ast, EXIT_FAIL, "minishell: pipe");
	perform_sub_proc(ast, ast_node, LEFT, fds);
	if (!prop->is_piped)
		close(fds[1]);
	perform_sub_proc(ast, ast_node, RIGHT, fds);
	if (!prop->is_piped)
		close(fds[0]);
	waitpid(prop->pids[LEFT], NULL, 0);
	waitpid(prop->pids[RIGHT], &status, 0);
	status = return_process_res(status);
	return (status);
}
