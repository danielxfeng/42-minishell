/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_supp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:37:48 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 21:19:48 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

// @brief return the error when there is an option.
//
// @param ast: the pointer to ast tree.
// @param prop: the property of node.
// @return the status code.
int	check_option(t_ast *ast, t_cmd_prop *prop)
{
	if (prop->size < 2)
		return (EXIT_OK);
	if (ft_strlen(ast->tokens[prop->start + 1]) > 1 && ast->tokens[prop->start
			+ 1][0] == '-')
	{
		ft_putstr_fd(ast->tokens[prop->start], 2);
		ft_putstr_fd(": invalid option -- \'", 2);
		ft_putchar_fd(ast->tokens[prop->start + 1][1], 2);
		ft_putstr_fd("\'\n", 2);
		return (EXIT_INVALID_OPTION);
	}
	return (EXIT_OK);
}

static char	*env_set_helper(t_ast *ast, char *key, char **dir)
{
	char	*buf;
	char	*env_item;
	
	buf = getcwd(NULL, 0);
	if (!buf)
		exit_with_err(&ast, EXIT_FAIL, "minishell: getcwd");
	env_item = ft_strjoin(key, buf);
	free(buf);
	if (!env_item)
	{
		free(dir);
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	}
	return (env_item);
}

// @brief help the cd to handle the directory switch.
//
// @param ast: the pointer to ast tree.
// @param prop: the property of node.
// @param dir: the path to switch.
// @return the status code.
int	cd_helper(t_ast *ast, t_cmd_prop *prop, char *dir)
{
	char	*env_item;
	
	env_item = env_set_helper(ast, "OLDPWD=", &dir);
	if (chdir(dir) != 0)
	{
		free(dir);	
		return (return_prt_err(EXIT_FAIL, "minishell: cd",
				ast->tokens[prop->start + 1], NULL));
	}
	env_set(ast->env, env_item);
	free(env_item);
	env_item = env_set_helper(ast, "PWD=", &dir);
	env_set(ast->env, env_item);
	free(env_item);
	free(dir);
	return (EXIT_OK);
}
