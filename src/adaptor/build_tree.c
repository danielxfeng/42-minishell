/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 11:36:06 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 19:26:17 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include "../include/executor.h"
#include <unistd.h>

// @brief to build a cmd node.
//
// A cmd node is a leaf.
//
// @param tree: the ast tree.
// @param node: the pointer to the node to be created.
// @param left: the left index of tokens.
// @param right: the right index of tokens.
static void build_cmd_node(t_ast *tree, t_ast_node **node, int left, int right)
{
    if (left > right)
        return  ;
    *node = create_cmd_node(tree, left, right - left + 1);
}

// @brief helper function for build_red_node.
//
// @param tree: the ast tree.
// @param params: an array of indexes.
// 0 curr index of tokens, 1 left index, 2 right index.
// @param is_in: is < or >.
// @param is_single: is << or <.
static t_ast_node *build_red_node_helper(t_ast *tree, int *params, bool is_in,
    bool is_single)
{
    t_ast_node  *node;
    
    node = create_red_node(tree, params[0], is_in, is_single);
    build_cmd_node(tree, &(node->left), params[1], params[0] - 2);
    build_red_node(tree, &(node->right), params[0] + 2, params[2]);
    return (node);
}

// @brief to build a red node.
//
// A red node has only 1 child.
// When not found: we try to find a cmd node.
// When found:
// We iterate from left to right, so there is no left red node.
// Then we try to find a cmd in the left, and red in the right.
//
// @param tree: the ast tree.
// @param node: the pointer to the node to be created.
// @param left: the left index of tokens.
// @param right: the right index of tokens.
static void build_red_node(t_ast *tree, t_ast_node **node, int left, int right)
{
    int curr;
    int params[3];

    if (left > right)
        return  ;
    curr = left;
    params[1] = left;
    params[2] = right;
    while (curr++ <= right)
    {
        params[0] = curr - 1;
        if (ft_strncmp(tree->tokens[curr - 1], "<", 2) == 0)
            *node = build_red_node_helper(tree, params, true, true);
        else if (ft_strncmp(tree->tokens[curr - 1], "<<", 3) == 0)
            *node = build_red_node_helper(tree, params, true, false);
        else if (ft_strncmp(tree->tokens[curr - 1], ">", 2) == 0)
            *node = build_red_node_helper(tree, params, false, true);
        else if (ft_strncmp(tree->tokens[curr - 1], ">>", 3) == 0)
            *node = build_red_node_helper(tree, params, false, false);
        else
            continue;       
        return  ;
    }
    build_cmd_node(tree, node, left, right);    
}

// @brief to build a pipe node.
//
// A pipe node has 2 children.
// When not found: we try to find a red node.
// When found:
// We iterate from right to left, so there is no right pipe node.
// Then we try to find another pipe in the left, and red in the right.
//
// @param tree: the ast tree.
// @param node: the pointer to the node to be created.
// @param left: the left index of tokens.
// @param right: the right index of tokens.
static void build_pipe_node(t_ast *tree, t_ast_node **node, int left, int right)
{
    int curr;

    if (left > right)
        return  ;
    curr = right;
    while (curr >= left)
    {
        if (ft_strncmp(tree->tokens[curr], "|", 2) == 0 ||
        ft_strncmp(tree->tokens[curr], "||", 3) == 0)
        {
            *node = create_pipe_node(tree);
            build_pipe_node(tree, &((*node)->left), left, curr - 1);
            build_red_node(tree, &((*node)->right), curr + 1, right);
            return  ;
        }
        --curr;
    }
    build_red_node(tree, node, left, right);
}

// @brief build the ast by given array of tokens.
//
// The pipeline is:
// 1. parse the pipe node (from right to left).
// 2. parse the red node (from left to right).
// 3. parse the cmd mode.
//
// Expamle:
// cmd < infile < infile2 > outfile0 | cmd2 | cmd3 > outfile | cmd4 > outfile2 > outfile3
//                                         Pipe3
//                              /                      /
//                            Pipe2              Red(> outfile2)
//                       /       /                   /
//                   Pipe1     Red(> outfile)   Red(> outfile3)
//                   /   /         /                /
//       Red(< infile1)  Cmd2    Cmd3             Cmd4
//         /               
//    Red(< infile2)
//      /
//  Red(> outfile0)
//   /
//  Cmd
// 
// @param ast: the pointer to ast.
// @param tokens: the array of tokens.
void    build_tree(char **tokens, int tk_size)
{
    t_ast *tree;

    tree = create_ast(tokens, tk_size);
    tree->fd_in = dup(STDIN_FILENO);
    tree->fd_out = dup(STDOUT_FILENO);
    if (tree->fd_in == -1 || tree->fd_out == -1)
        exit_with_err(&tree, EXIT_FAIL, "minishell: dup");
    build_pipe_node(tree, &(tree->root), 0, tk_size - 1);
}
