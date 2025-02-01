/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_close.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:58:36 by Xifeng            #+#    #+#             */
/*   Updated: 2024/12/10 19:16:32 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../pipe_x.h"
#include <stdlib.h>

// Helper function to free a pipe node.
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

// Helper function to free a cmd node.
void	close_cmd_node(t_ast_node *node)
{
	t_cmd_prop	*prop;
	int			i;

	prop = (t_cmd_prop *)node->prop;
	if (prop)
	{
		if (prop->full_cmd)
		{
			free(prop->full_cmd);
			prop->full_cmd = NULL;
		}
		if (prop->args)
		{
			i = 0;
			while (prop->args[i])
			{
				free(prop->args[i]);
				prop->args[i++] = NULL;
			}
			free(prop->args);
			prop->args = NULL;
		}
		free(prop);
		node->prop = NULL;
	}
}

// Helper function to free a red node.
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

// Helper function to free a AST node.
static void	free_ast_node(t_ast_node *node)
{
	if (node)
	{
		if (node->left)
		{
			free_ast_node(node->left);
			node->left = NULL;
		}
		if (node->right)
		{
			free_ast_node(node->right);
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
	int	i;

	i = 0;
	if (ast && *ast)
	{
		if ((*ast)->root)
			free_ast_node((*ast)->root);
		if ((*ast)->path)
		{
			while (((*ast)->path)[i])
			{
				free(((*ast)->path)[i]);
				((*ast)->path)[i++] = NULL;
			}
			free((*ast)->path);
			(*ast)->path = NULL;
		}
		free(*ast);
		*ast = NULL;
	}
}
