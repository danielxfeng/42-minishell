/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:25:11 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/07 18:51:51 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <stdlib.h>

// @brief `echo` with option `-n`
int	cmd_echo(t_ast *ast, t_cmd_prop *prop)
{
	return (0);
}

// @brief `cd` with only a relative or absolute path
int	cmd_cd(t_ast *ast, t_cmd_prop *prop)
{
	return (0);
}

// @brief `pwd` with no options
int	cmd_pwd(t_ast *ast, t_cmd_prop *prop)
{
	char *home;

    if (prop->size != 1)
        return_with_err(?, EXIT_FAIL, "minishell: pwd: ");
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
    if (prop->size != 1)
        return_with_err(?, EXIT_FAIL, "minishell: exit: ");
    close_ast(&ast);
    exit(EXIT_OK);
}
