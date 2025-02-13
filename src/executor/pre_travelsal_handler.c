/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_travelsal_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:23:57 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/13 19:13:40 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

int	here_doc_handler(t_ast *ast, t_red_prop *prop);
int	open_file_helper(t_ast *ast, t_red_prop *prop);

// @brief the pre-travelsal of the pipe node.
int	pre_pipe_handler(t_ast *ast, t_ast_node *node)
{
	node->left->node_pre_handler(ast, node->left);
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
	t_red_prop	*prop;

	prop = (t_red_prop *)node->prop;
	if (node->left)
		node->left->node_pre_handler(ast, node->left);
	if (prop->is_in && !(prop->is_single))
		res = here_doc_handler(ast, prop);
	else
		res= open_file_helper(ast, prop);
	return (res);
}
