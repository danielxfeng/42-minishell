/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:25:11 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 08:37:59 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include "../include/executor.h"
#include <stdlib.h>
#include <errno.h>

// @brief `echo` with option `-n`
int	cmd_echo(t_ast *ast, t_cmd_prop *prop)
{
	return (0);
}

// @brief `cd` with only a relative or absolute path
int	cmd_cd(t_ast *ast, t_cmd_prop *prop)
{
	if (prop->size != 1)
		return_prt_err(EXIT_FAIL, "minishell: exit: too many arguments\n");
	
	if (chdir(ast->tokens[prop->start + 1]) != 0)
		return_with_err(INVALID_ERR_NO, EXIT_FAIL, "");
	return (EXIT_OK);
}

// @brief `pwd` with no options
int	cmd_pwd(t_ast *ast, t_cmd_prop *prop)
{
	char *home;

    if (prop->size != 1)
	return_prt_err(EXIT_FAIL, "minishell: exit: too many arguments\n");
	home = getenv("HOME");
	printf("%s\n", home);
}

// @brief `export` with no options
int	cmd_export(t_ast *ast, t_cmd_prop *prop)
{
	return (0);
}

// @brief `unset` with no options
int	cmd_unset(t_ast *ast, t_cmd_prop *prop)
{
	return (0);
}

// @brief `env` with no options or arguments
int	cmd_env(t_ast *ast, t_cmd_prop *prop)
{
	return (0);
}

// @brief `exit` with no options
int	cmd_exit(t_ast *ast, t_cmd_prop *prop)
{
	int status;
	
    if (prop->size != 1 && prop->size != 2)
        return_prt_err(EXIT_FAIL, "minishell", "exit", "too many arguments");
	status = EXIT_OK;
	if (prop->size == 2 && (ft_strcmp(ast->tokens[prop->start + 1], "0") != 0 &&
		ft_strcmp(ast->tokens[prop->start + 1], "+0") != 0 && 
		ft_strcmp(ast->tokens[prop->start + 1], "-0") != 0))
	{
		status = ft_atoi(ast->tokens[prop->start + 1]);
		if (status == 0)
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(ast->tokens[prop->start + 1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			status = EXIT_FAIL;
		}
	}
	close_ast(&ast);
    exit(status);
}
