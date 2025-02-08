/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_printer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:15:40 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/07 14:32:54 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>

void		print_curr_msg(t_ast_node *node, t_ast_node *curr, char *msg);

// @brief the printer of a pipe node.
void	print_pipe_node(t_ast *ast, t_ast_node *node, t_ast_node *curr,
		char *msg)
{
	t_pipe_prop	*prop;

	prop = (t_pipe_prop *)node->prop;
	printf("-PIP, fd0: %d, fd1: %d, pids: %d, %d ", prop->fds[0], prop->fds[1],
		prop->pids[0], prop->pids[1]);
	print_curr_msg(node, curr, msg);
}

// @brief the printer of a command node.
void	print_cmd_node(t_ast *ast, t_ast_node *node, t_ast_node *curr,
		char *msg)
{
	t_cmd_prop	*prop;
	int			i;

	prop = (t_cmd_prop *)node->prop;
	printf("-CMD, pid: %d, full_cmd: %d, cmd: ", prop->pid, prop->full_cmd);
	i = prop->start;
	while (i < prop->size)
		printf("%s ", ast->tokens[i]);
	print_curr_msg(node, curr, msg);
}

// @brief the printer of a red node.
void	print_red_node(t_ast *ast, t_ast_node *node, t_ast_node *curr,
		char *msg)
{
	t_red_prop	*prop;

	prop = (t_red_prop *)node->prop;
	printf("-RED, file: %s, fd: %d, is_in: %d, is_single: %d ",
		ast->tokens[prop->idx], prop->fd, prop->is_in, prop->is_single);
	print_curr_msg(node, curr, msg);
}

// @brief a helper function to print the ast tree.
static void	print_ast_helper(t_ast *ast, t_ast_node *node, t_ast_node *curr,
		char *msg)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	print_ast_helper(ast, node->left, curr, msg);
	if (node->node_printer)
		node->node_printer(ast, node, curr, msg);
	print_ast_helper(ast, node->right, curr, msg);
}

// @brief Print the AST tree by In-order Traversal when DEBUG Flag is set.
//
// @param ast: the pointer to ast tree.
// @param curr: the current node, prints a *** to identify the current node.
// @param msg: the debug message to be printed.
void	debug_print_ast(t_ast *ast, t_ast_node *curr, char *msg)
{
	int	i;

	if (!DEBUG_FLAG)
		return ;
	printf("-----------------------AST Tree-----------------------\n");
	if (!ast)
	{
		printf("NULL\n");
		return ;
	}
	printf("Tokens: ");
	i = 0;
	while (i < ast->tk_size)
		printf("%s ", ast->tokens[i++]);
	printf("\n");
	print_ast_helper(ast, ast->root, curr, msg);
	ft_printf("------------------------------------------------------\n");
}
