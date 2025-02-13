/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_travelsal_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:23:57 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/13 16:33:12 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

int	here_doc_handler(t_ast *ast, t_red_prop *prop);
int	open_file_helper(t_ast *ast, t_red_prop *prop);

// @brief the pre-travelsal of the pipe node.
int	pre_pipe_handler(t_ast *ast, t_ast_node *node)
{
	node->left->node_pre_handler(ast, node);
	return (node->right->node_pre_handler(ast, node));
}

// @brief the pre-travelsal of the cmd node.
int	pre_cmd_handler(t_ast *ast, t_ast_node *node)
{
	(void)ast;
	(void)node;
	return (EXIT_OK);
}

// @brief the pre-travelsal of the red node.
int	pre_red_handler(t_ast *ast, t_ast_node *node)
{
	t_red_prop	*prop;

	prop = (t_red_prop *)node->prop;
	if (prop->is_in && !(prop->is_single))
		here_doc_handler(ast, prop);
	else
		open_file_helper(ast, prop);
	return (node->left->node_pre_handler(ast, node));
}
