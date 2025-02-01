/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_factory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:41:30 by Xifeng            #+#    #+#             */
/*   Updated: 2024/12/13 13:57:17 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../pipe_x.h"

// Constructor of a AST node for PIPE.
t_ast_node	*create_pipe_node(t_ast *ast)
{
	t_ast_node	*node;
	t_pipe_prop	*prop;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		exit_prog(&ast, "create_pipe_node()", MALLOC_ERR, EXIT_FAILURE);
	prop = ft_calloc(1, sizeof(t_pipe_prop));
	if (!(prop))
	{
		free(node);
		exit_prog(&ast, "create_pipe_node()", MALLOC_ERR, EXIT_FAILURE);
	}
	prop->fds[0] = -1;
	prop->fds[1] = -1;
	node->type = PIPE;
	node->prop = prop;
	node->node_handler = pipe_handler;
	node->node_closer = close_pipe_node;
	return (node);
}

// Constructor of a AST node for CMD.
t_ast_node	*create_cmd_node(t_ast *ast, char *argv)
{
	t_ast_node	*node;
	t_cmd_prop	*prop;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		exit_prog(&ast, "create_cmd_node()", MALLOC_ERR, EXIT_FAILURE);
	prop = ft_calloc(1, sizeof(t_cmd_prop));
	if (!prop)
	{
		free(node);
		exit_prog(&ast, "create_cmd_node()", MALLOC_ERR, EXIT_FAILURE);
	}
	prop->args = ft_split(argv, ' ');
	if (!(prop->args))
	{
		free(node);
		free(prop);
		exit_prog(&ast, "ft_split()", MALLOC_ERR, EXIT_FAILURE);
	}
	node->type = CMD;
	node->prop = prop;
	node->node_handler = cmd_handler;
	node->node_closer = close_cmd_node;
	return (node);
}

// Constructor of a AST node for REDIRECT.
t_ast_node	*create_red_node(t_ast *ast, char *file_name, bool is_in,
		bool is_single)
{
	t_ast_node	*node;
	t_red_prop	*prop;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		exit_prog(&ast, "create_red_node()", MALLOC_ERR, EXIT_FAILURE);
	prop = ft_calloc(1, sizeof(t_red_prop));
	if (!prop)
	{
		free(node);
		exit_prog(&ast, "create_red_node()", MALLOC_ERR, EXIT_FAILURE);
	}
	prop->file_name = file_name;
	prop->is_in = is_in;
	prop->is_single = is_single;
	prop->fd = -1;
	node->type = RED;
	node->prop = prop;
	node->node_handler = red_handler;
	node->node_closer = close_red_node;
	return (node);
}

// Constructor of AST tree.
t_ast	*create_ast(char **envp, char **path)
{
	t_ast	*ast;

	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
		exit_prog(&ast, "create_ast()", MALLOC_ERR, EXIT_FAILURE);
	ast->root = NULL;
	ast->envp = envp;
	ast->path = path;
	return (ast);
}

// Build the AST tree.
// We trvesal the arguments from right to left.
// 1 The last 2 are the right node of root, which are
//   a `RED` node with the last `CMD`.
// 2 We iterate the rest nodes until 2 nodes left, we create
//   a `PIPE` node at `curr`'s left, and a `CMD` node as its right child.
// 3 The first 2 are the leftmost nodes of the tree, which are
//   a `RED` node with the 1-st `CMD`.
// For example:
// ./pipex infile cmd1 cmd2 cmd3 outfile
// == < infile cmd1 | cmd2 | cmd3 > outfile
//               PIPE
//              /    /
//         PIPE     RED(> outfile)
//        /    \              /
// RED(<infile) CMD(cmd2)  CMD(cmd3)
//       /
//   CMD(cmd1)
void	build_ast(t_ast *ast, int len, char **args, bool is_single)
{
	int			i;
	t_ast_node	*curr;

	ast->root = create_pipe_node(ast);
	ast->root->right = create_red_node(ast, args[len - 1], false, is_single);
	ast->root->right->left = create_cmd_node(ast, args[len - 2]);
	ast->fd_in = dup(STDIN_FILENO);
	if (ast->fd_in < 0)
		exit_prog(&ast, "build_ast()", ORIGIN_DUP_ERR, EXIT_FAILURE);
	ast->fd_out = dup(STDOUT_FILENO);
	if (ast->fd_out < 0)
		exit_prog(&ast, "build_ast()", ORIGIN_DUP_ERR, EXIT_FAILURE);
	i = len - 3;
	curr = ast->root;
	while (i > 1)
	{
		curr->left = create_pipe_node(ast);
		curr = curr->left;
		curr->right = create_cmd_node(ast, args[i]);
		--i;
	}
	curr->left = create_red_node(ast, args[0], true, is_single);
	curr->left->right = create_cmd_node(ast, args[1]);
}
