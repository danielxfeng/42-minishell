/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_close.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:58:36 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/04 20:48:17 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>

void close_path(t_ast *ast);
void close_tokens(t_ast *ast);

// @brief the destructor of a pipe node.
void	close_pipe_node(t_ast_node *node)
{
	t_pipe_prop	*prop;

	prop = (t_pipe_prop *)node->prop;
	if (prop)
	{
		if (prop->fds[0] >= 0)
			close(prop->fds[0]);
		if (prop->fds[1] >= 0)
			close(prop->fds[1]);
		free(prop);
		node->prop = NULL;
	}
}

// @brief the destructor of a cmd node.
void	close_cmd_node(t_ast_node *node)
{
	t_cmd_prop	*prop;
	int			i;

	prop = (t_cmd_prop *)node->prop;
	if (prop)
	{
		free(prop);
		node->prop = NULL;
	}
}

// @brief the destructor of a red node.
void	close_red_node(t_ast_node *node)
{
	t_red_prop	*prop;

	prop = (t_red_prop *)node->prop;
	if (prop)
	{
		if (prop->fd >= 0)
			close(prop->fd);
		free(prop);
		node->prop = NULL;
	}
}

// @brief the helper function of the destructor of ast node.
static void	close_ast_node(t_ast_node *node)
{
	if (node)
	{
		if (node->left)
		{
			close_ast_node(node->left);
			node->left = NULL;
		}
		if (node->right)
		{
			close_ast_node(node->right);
			node->right = NULL;
		}
		if (node->node_closer)
			node->node_closer(node);
		free(node);
	}
}

// @brief deconstructor of the AST tree.
//
// @param ast: the pointer to pointer of an ast tree.
void	close_ast(t_ast **ast)
{
	if (ast && *ast)
	{
		if ((*ast)->root)
			close_ast_node((*ast)->root);
		close_path(*ast);
		close_tokens(*ast);
		free(*ast);
		*ast = NULL;
	}
}
