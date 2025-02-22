/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:16:18 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 10:57:49 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// @brief set the skip flag recurrsivly.
//
// In one sub-process, for each redirecton direction,
// only rightmost `red` node will be redirected,
//
// @param node: the current node.
// @param prop: the property of the node.
// @param is_in: the direction of `red`, is it a < ? or > ?
void	set_skip_flag(t_ast_node *node, t_red_prop *prop, bool is_in)
{
	if (!(node->left) || !(node->left->type == RED))
		return ;
	prop = node->left->prop;
	if (prop->is_skip && prop->is_in == is_in)
		return ;
	if (prop->is_in == is_in)
		prop->is_skip = true;
	set_skip_flag(node->left, prop, is_in);
}

// @brief the executor of a `red` node.
//
// The `red` node has 1 possible left child for cmd or another red.
//
// The pipeline:
// 1 Perform a Pre-order travelsal to set the `is_skip` sign at first.
// Because for each direction, only right most node is redirected.
// 2 Perform the Pre-order travelsal to apply the redirection.
//
// @param ast: the pointer to the ast tree.
// @param ast_node: the `red` node.
// @return the status code.
int	red_handler(t_ast *ast, t_ast_node *ast_node)
{
	t_red_prop	*prop;
	int			res;

	res = EXIT_OK;
	debug_print_ast(ast, ast_node, "Exec Red.");
	prop = (t_red_prop *)ast_node->prop;
	if (!prop->is_skip)
		set_skip_flag(ast_node, prop, prop->is_in);
	if (prop->fd < 0)
		return (prop->status);
	if (!prop->is_skip)
	{
		if ((prop->is_in && dup2(prop->fd, STDIN_FILENO) < 0) || (!(prop->is_in)
				&& dup2(prop->fd, STDOUT_FILENO) < 0))
			exit_with_err(&ast, EXIT_FAIL, "minishell: dup2");
	}
	if (ast_node->left)
		res = ast_node->left->node_handler(ast, ast_node->left);
	return (res);
}
