/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 18:45:09 by Xifeng            #+#    #+#             */
/*   Updated: 2024/12/10 21:41:26 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf/ft_printf.h"
#include "../pipe_x.h"

static void	print_pipe_helper(t_ast_node *node)
{
	t_pipe_prop	*prop;

	prop = (t_pipe_prop *)node->prop;
	ft_printf("-PIP, fd0: %d, fd1: %d, pids: %d, %d\n", prop->fds[0],
		prop->fds[1], prop->pids[0], prop->pids[1]);
}

static void	print_cmd_helper(t_ast_node *node)
{
	t_cmd_prop	*prop;
	int			i;

	prop = (t_cmd_prop *)node->prop;
	ft_printf("-CMD, full path cmd: %s, args: ", prop->full_cmd);
	i = 0;
	while (prop->args[i])
		ft_printf("%s, ", prop->args[i++]);
	ft_printf("\n");
}

static void	print_red_helper(t_ast_node *node)
{
	t_red_prop	*prop;

	prop = (t_red_prop *)node->prop;
	ft_printf("-RED, file: %s, fd: %d, is_in: %d, is_single: %d\n",
		prop->file_name, prop->fd, prop->is_in, prop->is_single);
}

static void	print_ast_helper(t_ast_node *node, int layer)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	print_ast_helper(node->left, layer + 1);
	while (i++ < layer)
		ft_printf("   ");
	if (node->type == PIPE)
		print_pipe_helper(node);
	else if (node->type == CMD)
		print_cmd_helper(node);
	else if (node->type == RED)
		print_red_helper(node);
	print_ast_helper(node->right, layer + 1);
}

// Print the AST tree by In-order Traversal.
void	print_ast(t_ast *ast)
{
	int	i;

	ft_printf("-----------------------AST Tree-----------------------\n");
	if (!ast)
	{
		ft_printf("NULL\n");
		return ;
	}
	ft_printf("ENVP: ");
	i = 0;
	while (ast->path && ast->path[i])
		ft_printf("%s, ", ast->path[i++]);
	ft_printf("\n");
	print_ast_helper(ast->root, 0);
	ft_printf("------------------------------------------------------\n");
}
