/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 11:36:06 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/17 17:46:15 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <unistd.h>

void				build_red_node(t_ast *tree, t_ast_node **node, int left,
						int right);

// @brief to build a cmd node.
//
// A cmd node is a leaf.
//
// @param tree: the ast tree.
// @param node: the pointer to the node to be created.
// @param left: the left index of tokens.
// @param right: the right index of tokens.
static void	build_cmd_node(t_ast *tree, t_ast_node **node, int left, int right)
{
	if (left > right)
		return ;
	*node = create_cmd_node(tree, left, right - left + 1);
}

// @brief helper function for build_red_node.
//
// @param tree: the ast tree.
// @param params: an array of indexes.
// 0 curr index of tokens, 1 left index, 2 right index.
// @param is_in: is < or >.
// @param is_single: is << or <.
// @return the created red node.
static t_ast_node	*build_red_node_helper(t_ast *tree, int *params, bool is_in,
		bool is_single)
{
	t_ast_node	*node;

	node = create_red_node(tree, params[0] + 1, is_in, is_single);
	build_red_node(tree, &(node->left), params[1], params[0] - 1);
	return (node);
}

// @brief to build a red node.
//
// A red node has only left child which can be another red node or cmd node.
//
// We iterate from right to left, try to find a red node.
// When not found:
//   - restart the iteration, and try to find a cmd node.
// When found:
//   - We try to find another red node on the left.
//
// @param tree: the ast tree.
// @param node: the pointer to the node to be created.
// @param left: the left index of tokens.
// @param right: the right index of tokens.
void	build_red_node(t_ast *tree, t_ast_node **node, int left, int right)
{
	int	curr;
	int	params[3];

	if (left > right)
		return ;
	curr = right + 1;
	params[1] = left;
	params[2] = right;
	while (--curr >= left)
	{
		params[0] = curr;
		if (ms_strcmp(tree->tokens[curr], "<") == 0)
			*node = build_red_node_helper(tree, params, true, true);
		else if (ms_strcmp(tree->tokens[curr], "<<") == 0)
			*node = build_red_node_helper(tree, params, true, false);
		else if (ms_strcmp(tree->tokens[curr], ">") == 0)
			*node = build_red_node_helper(tree, params, false, true);
		else if (ms_strcmp(tree->tokens[curr], ">>") == 0)
			*node = build_red_node_helper(tree, params, false, false);
		else
			continue ;
		return ;
	}
	build_cmd_node(tree, node, left, right);
}

// @brief to build a pipe node.
//
// A pipe node has 2 children.
// The left child can be another pipe node, a red node, or a cmd node.
// The right child can be a red node, or a cmd node.
//
// We iterate from right to left, try to find a pipe node.
// When not found:
//   - restart the iteration, and try to find a red node.
// When found:
//   - We try to find a red node on the right.
//   - And we try to find another pipe on the left.
//   - Because the iteration was from right to left,
//     so there is no pipe node on the right, but may still have some
//     on the left.
//
// @param tree: the ast tree.
// @param node: the pointer to the node to be created.
// @param left: the left index of tokens.
// @param right: the right index of tokens.
static void	build_pipe_node(t_ast *tree, t_ast_node **node, int left, int right)
{
	int	curr;

	if (left > right)
		return ;
	curr = right;
	while (curr >= left)
	{
		if (ms_strcmp(tree->tokens[curr], "|") == 0)
		{
			*node = create_pipe_node(tree);
			build_pipe_node(tree, &((*node)->left), left, curr - 1);
			build_red_node(tree, &((*node)->right), curr + 1, right);
			return ;
		}
		--curr;
	}
	build_red_node(tree, node, left, right);
}

// @brief build the ast by given array of tokens.
//
// The pipeline is:
// 1. parsing all the pipe nodes (from right to left).
// 2. parsing all the the red nodes (from right to left).
// 3. parsing all the cmd modes.
//
// Expamle:
// cmd param1 < infile1 < infile2 > outfile0 | cmd2 | cmd3 > outfile1 | cmd4 > outfile2 > outfile3
//                                         Pipe3
//                              /                      /
//                            Pipe2              Red(> outfile3)
//                       /       /                   /
//                   Pipe1     Red(> outfile1)   Red(> outfile2)
//                   /   /         /                /
//       Red(> outfile0)  Cmd2    Cmd3             Cmd4
//         /
//    Red(< infile2)
//      /
//  Red(> infile1)
//   /
//  Cmd param1
//
// @param ast: the pointer to ast.
// @param tokens: the array of tokens.
t_ast	*build_tree(char **tokens, int tk_size)
{
	t_ast	*tree;

	tree = create_ast(tokens, tk_size);
	tree->fd_in = dup(STDIN_FILENO);
	tree->fd_out = dup(STDOUT_FILENO);
	if (tree->fd_in == -1 || tree->fd_out == -1)
		exit_with_err(&tree, EXIT_FAIL, "minishell: dup");
	build_pipe_node(tree, &(tree->root), 0, tk_size - 1);
	//debug_print_ast(tree, tree->root, "Build tree");
	return (tree);
}
