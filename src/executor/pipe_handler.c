/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 19:42:01 by Xifeng            #+#    #+#             */
/*   Updated: 2024/12/13 13:09:31 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipe_x.h"
#include <sys/wait.h>

// Helper function to deal with fds in sub-process.
//
// 1 Redirects the `stdin` or `stdout` to to the pipe's corresponding end.
// 2 Closes the redirected fd because it has been redirected.
// 3 Closes another fd because we will not use it.
//
// The `fds` of parent-proc will be copied when `fork`.
// So sub-process has a copy of parent-proc's `fds`.
// And, they are copied, so the `close` doesnot affect the `fds` in parent-proc.
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
		exit_prog(&ast, "dup2()", DUP_ERR, EXIT_FAILURE);
	close(prop->fds[0]);
	close(prop->fds[1]);
}

// Helper function to handle the sub_process.
//
// 1 Start a new process by syscall `fork`.
// Note The memory state of the current process will be COPIED to sub-process,
// which means in sub-proc, we can access `prop`, but it's a copied one,
// any modification in sub-proc does not affect the `prop` in parent-process.
//
// -----NOW WE ARE IN SUB-PROCESS------
// 2 Handle the pipe in sub-process.
// 3 Execute the child node here and return.
//
// Note: before return, we have to free resources which are copied from
// parent.
//
// Note we use `pid` to determine where we are (in SUB-PROCESS,
// or PARENT-PROCESS)
// We can do this because of this expression `prop->pids[direction] = fork()`
// As I mentioned before, sub-proc gets a stand alone copy when `fork`,
// which means
//  - In sub-proc, the process is created before `fork`'s return
//    so `pid` cannot be updated.
//  - In parent-proc. the `pid` has been updated by `fork`'s return.
static void	perform_sub_proc(t_ast *ast, t_ast_node *node, t_pipe_prop *prop,
		int direction)
{
	t_ast_node	*child;

	child = node->left;
	if (direction)
		child = node->right;
	prop->pids[direction] = fork();
	if (prop->pids[direction] < 0)
		exit_prog(&ast, "fork()", FORK_ERR, EXIT_FAILURE);
	if (prop->pids[direction] == 0)
	{
		handle_sub_fds(ast, prop, direction);
		child->node_handler(ast, child);
		exit_prog(&ast, NULL, NULL, EXIT_SUCCESS);
	}
}

// Handle the operation of PIPE.
// Pipe is a information channel like `file` or `stdin` across the processes,
// so it can be represented as 2 `fds`.
//
// The main logic is:
// 1 Call the syscall `pipe` to assign 2 `fds` which represents the pipe.
// 2 Start 2 sub-processes to run the left and right children.
// 3 Wait until 2 sub-processces stop and then clear the resources.
//
// Note the process is a recurrsive call by hybrid pre-post traversal,
// which means mid->left->right->mid
// 
// Note We apply `waitpid` to wait for the sub-process return.
// When `RIGHT` returns, it closes the pipe before quit the process.
// Therefore, even if `LEFT` is in an infinity loop, it will still quit 
// for `write` is error.
int	pipe_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_pipe_prop	*prop;
	int			status;

	prop = (t_pipe_prop *)ast_node->prop;
	if (pipe(prop->fds) < 0)
		exit_prog(&ast, "pipe()", PIPE_ERR, EXIT_FAILURE);
	perform_sub_proc(ast, ast_node, prop, LEFT);
	close(prop->fds[1]);
	perform_sub_proc(ast, ast_node, prop, RIGHT);
	close(prop->fds[0]);
	waitpid(prop->pids[LEFT], &status, 0);
	waitpid(prop->pids[RIGHT], &status, 0);
	status = return_process_res(status);
	prop->fds[1] = -1;
	prop->fds[0] = -1;
	return (status);
}
