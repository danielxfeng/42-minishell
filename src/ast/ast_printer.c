/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_printer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:15:40 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/13 16:39:55 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>

void		print_curr_msg(t_ast_node *node, t_ast_node *curr);
void		print_level(int level);

// @brief the printer of a pipe node.
void	print_pipe_node(t_ast *ast, t_ast_node *node, t_ast_node *curr,
		int level)
{
	t_pipe_prop	*prop;

	(void)ast;
	prop = (t_pipe_prop *)node->prop;
	print_level(level);
	printf("-PIP(%d), is_piped: %d, fd1: %d, pids: %d, %d ", level, prop->is_piped,
	prop->pids[0], prop->pids[1]);
	print_curr_msg(node, curr);
}

// @brief the printer of a command node.
void	print_cmd_node(t_ast *ast, t_ast_node *node, t_ast_node *curr,
		int level)
{
	t_cmd_prop	*prop;
	int			i;

	prop = (t_cmd_prop *)node->prop;
	print_level(level);
	printf("-CMD(%d), pid: %d, full_cmd: %s, cmd: ", level, prop->pid,
		prop->full_cmd);
	i = 0;
	while (i < prop->size)
		printf("%s ", ast->tokens[i++ + prop->start]);
	print_curr_msg(node, curr);
}

// @brief the printer of a red node.
void	print_red_node(t_ast *ast, t_ast_node *node, t_ast_node *curr,
		int level)
{
	t_red_prop	*prop;

	prop = (t_red_prop *)node->prop;
	print_level(level);
	printf("-RED(%d), file: %s, fd: %d, is_in: %d, is_single: %d, ", level,
		ast->tokens[prop->idx], prop->fd, prop->is_in, prop->is_single);
	printf("is_skip: %d ", prop->is_skip);
	print_curr_msg(node, curr);
}

// @brief a helper function to print the ast tree.
static void	print_ast_helper(t_ast *ast, t_ast_node *node, t_ast_node *curr,
		int level)
{
	if (!node)
		return ;
	print_ast_helper(ast, node->left, curr, level + 1);
	if (node->node_printer)
		node->node_printer(ast, node, curr, level);
	print_ast_helper(ast, node->right, curr, level + 1);
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
	printf("Message: %s\nTokens: ", msg);
	i = 0;
	while (i < ast->tk_size)
		printf("%s ", ast->tokens[i++]);
	printf("\n");
	print_ast_helper(ast, ast->root, curr, 0);
	printf("------------------------------------------------------\n");
}
