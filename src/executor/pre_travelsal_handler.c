/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_travelsal_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:23:57 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/05 18:04:48 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

int	here_doc_handler(t_ast *ast, t_red_prop *prop);
int	open_file_helper(t_ast *ast, t_red_prop *prop);

// @brief the pre-travelsal of the pipe node.
int	pre_pipe_handler(t_ast *ast, t_ast_node *node)
{
	if (node->left->node_pre_handler(ast, node->left) == 130)
		return (130);
	return (node->right->node_pre_handler(ast, node->right));
}

// @brief the pre-travelsal of the cmd node.
int	pre_cmd_handler(t_ast *ast, t_ast_node *node)
{
	(void)ast;
	(void)node;
	return (EXIT_OK);
}

// @brief the post-travelsal of the red node.
int	pre_red_handler(t_ast *ast, t_ast_node *node)
{
	int			res;
	bool		has_res_from_child;
	t_red_prop	*prop;

	has_res_from_child = false;
	prop = (t_red_prop *)node->prop;
	res = EXIT_OK;
	if (node->left && node->left->type == E_RED)
	{
		has_res_from_child = true;
		prop->status = node->left->node_pre_handler(ast, node->left);
	}
	if (prop->is_in && !(prop->is_single))
		res = here_doc_handler(ast, prop);
	else if (prop->status == EXIT_OK)
		res = open_file_helper(ast, prop);
	if (res == 130 || !has_res_from_child || (has_res_from_child
			&& prop->status == EXIT_OK))
		prop->status = res;
	return (prop->status);
}
