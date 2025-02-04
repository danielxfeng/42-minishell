/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_close.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:58:36 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/04 19:21:02 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>

void close_path(t_ast *ast);
void close_tokens(t_ast *ast);

// Helper function to free a pipe node.
static void	close_pipe_node(t_ast_node *node)
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

// Helper function to free a cmd node.
static void	close_cmd_node(t_ast_node *node)
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

// Helper function to free a red node.
static void	close_red_node(t_ast_node *node)
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

// Helper function to close a AST node.
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

// Deconstructor of the AST tree.
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
