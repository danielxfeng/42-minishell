/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_funcs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 08:12:16 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 10:57:05 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdio.h>

void	print_env_item_for_export(t_env_item *item);
void	print_env_item_for_env(t_env_item *item);
void	print_env(t_env *env, void (*print_func)(t_env_item *));
int		check_identifier(char *s);
int		check_option(t_ast *ast, t_cmd_prop *prop);

// @brief `export` with no options
// check 'man' page for more information.
// argc: prop->size;
// argv: ast->tokens[prop->start];
int	cmd_export(t_ast *ast, t_cmd_prop *prop)
{
	int		i;
	int		status;
	bool	ok;

	status = check_option(ast, prop);
	if (status != EXIT_OK)
		return (status);
	if (prop->size == 1)
	{
		print_env(ast->env, print_env_item_for_export);
		return (EXIT_OK);
	}
	i = 1;
	while (i < prop->size)
	{
		status = check_identifier(ast->tokens[prop->start + i]);
		if (status != EXIT_OK)
			return (status);
		ok = env_set(ast->env, ast->tokens[prop->start + i]);
		if (!ok)
			exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
		++i;
	}
	return (EXIT_OK);
}

// @brief `unset` with no options
// check 'man' page for more information.
// argc: prop->size;
// argv: ast->tokens[prop->start];
int	cmd_unset(t_ast *ast, t_cmd_prop *prop)
{
	int	i;
	int	status;

	status = check_option(ast, prop);
	if (status != EXIT_OK)
		return (status);
	i = 1;
	while (i < prop->size)
	{
		status = check_identifier(ast->tokens[prop->start + i]);
		if (status != EXIT_OK)
			return (status);
		env_remove(ast->env, ast->tokens[prop->start + i]);
		++i;
	}
	return (EXIT_OK);
}

// @brief `env` with no options or arguments
// check 'man' page for more information.
// argc: prop->size;
// argv: ast->tokens[prop->start];
int	cmd_env(t_ast *ast, t_cmd_prop *prop)
{
	int	status;

	status = check_option(ast, prop);
	if (status != EXIT_OK)
		return (status);
	print_env(ast->env, print_env_item_for_env);
	return (EXIT_OK);
}
