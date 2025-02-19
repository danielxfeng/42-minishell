/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_factory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:15:48 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/18 18:56:31 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>

// @brief constructor of a AST node for PIPE.
t_ast_node	*create_pipe_node(t_ast *ast)
{
	t_ast_node	*node;
	t_pipe_prop	*prop;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	prop = ft_calloc(1, sizeof(t_pipe_prop));
	if (!(prop))
	{
		free(node);
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	}
	node->type = PIPE;
	node->prop = prop;
	node->node_pre_handler = pre_pipe_handler;
	node->node_handler = pipe_handler;
	node->node_closer = close_pipe_node;
	node->node_printer = print_pipe_node;
	return (node);
}

// @brief constructor of a AST node for CMD.
t_ast_node	*create_cmd_node(t_ast *ast, int start, int size)
{
	t_ast_node	*node;
	t_cmd_prop	*prop;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	prop = ft_calloc(1, sizeof(t_cmd_prop));
	if (!prop)
	{
		free(node);
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	}
	prop->start = start;
	prop->size = size;
	node->type = CMD;
	node->prop = prop;
	node->node_pre_handler = pre_cmd_handler;
	node->node_handler = cmd_handler;
	node->node_closer = close_cmd_node;
	node->node_printer = print_cmd_node;
	return (node);
}

// @brief constructor of a AST node for REDIRECT.
t_ast_node	*create_red_node(t_ast *ast, int idx, bool is_in, bool is_single)
{
	t_ast_node	*node;
	t_red_prop	*prop;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	prop = ft_calloc(1, sizeof(t_red_prop));
	if (!prop)
	{
		free(node);
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	}
	prop->idx = idx;
	prop->is_in = is_in;
	prop->is_single = is_single;
	prop->fd = -1;
	node->type = RED;
	node->prop = prop;
	node->node_pre_handler = pre_red_handler;
	node->node_handler = red_handler;
	node->node_closer = close_red_node;
	node->node_printer = print_red_node;
	return (node);
}

// @brief constructor of AST tree.
//
// @param tokens: the string array of tokens.
// @param tk_size: the size of tokens.
// @param env: the pointer to env.
// @return the pointer to ast. Returns NULL when error on malloc.
t_ast	*create_ast(char **tokens, int tk_size, t_env *env)
{
	t_ast	*ast;
	int		i;

	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
	{
		i = 0;
		while (i < tk_size)
			free(tokens[i++]);
		free(tokens);
		tokens = NULL;
		close_env(&env);
		env = NULL;
		exit_with_err(NULL, EXIT_FAIL, "minishell: malloc");
	}
	ast->root = NULL;
	ast->tokens = tokens;
	ast->tk_size = tk_size;
	ast->fd_in = -1;
	ast->fd_out = -1;
	ast->env = env;
	return (ast);
}
