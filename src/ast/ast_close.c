/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_close.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:15:09 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/13 12:14:39 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>
#include <unistd.h>

void		close_tokens(t_ast *ast);
int			close_fds(t_ast *ast);

// @brief the destructor of a pipe node.
void	close_pipe_node(t_ast_node *node)
{
	t_pipe_prop	*prop;

	prop = (t_pipe_prop *)node->prop;
	if (prop)
	{
		free(prop);
		node->prop = NULL;
	}
}

// @brief the destructor of a cmd node.
void	close_cmd_node(t_ast_node *node)
{
	t_cmd_prop	*prop;
	//int			i;

	prop = (t_cmd_prop *)node->prop;
	if (prop)
	{
		if (prop->full_cmd)
			free(prop->full_cmd);
		prop->full_cmd = NULL;
		if (prop->argv)
			free(prop->argv);
		prop->argv = NULL;
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
// @return status code
int	close_ast(t_ast **ast)
{
	int	status;

	status = EXIT_OK;
	if (ast && *ast)
	{
		if ((*ast)->root)
			close_ast_node((*ast)->root);
		close_tokens(*ast);
		close_fds(*ast);
		free(*ast);
		*ast = NULL;
	}
	return (status);
}
