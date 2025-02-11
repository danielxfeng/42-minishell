/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:25:11 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/11 10:26:08 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// @brief `echo` with option `-n`
int	cmd_echo(t_ast *ast, t_cmd_prop *prop)
{
	int	i;

	bool line_break ;
	line_break = false;
	if (prop->size > 1 && (ft_strncmp(ast->tokens[prop->start + 1], "-n",
				3) == 0))
		line_break = true;
	i = 1;
	if (line_break)
		++i;
	while (i < prop->size)
	{
		printf("%s", ast->tokens[prop->start + i]);
		if (i + 1 < prop->size)
			printf(" ");
		++i;
	}
	if (!line_break)
		printf("\n");
	return (EXIT_OK);
}

// @brief `cd` with only a relative or absolute path
int	cmd_cd(t_ast *ast, t_cmd_prop *prop)
{
	char	*path;
	char	dir[PATH_MAX + 1];

	if (prop->size != 1 && prop->size != 2)
		return_prt_err(EXIT_FAIL, "minishell", "cd", "too many arguments");
	if (prop->size == 1)
	{
		path = getenv("HOME");
		if (!path)
		{
			if (getcwd(dir, sizeof(dir)) == NULL)
				return_prt_err(EXIT_FAIL, "minishell", "cd", NULL);
			path = dir;
		}
	}
	else
		path = ast->tokens[prop->start + 1];
	if (chdir(path) != 0)
		return_prt_err(EXIT_FAIL, "minishell: cd", path, NULL);
	return (EXIT_OK);
}

// @brief `pwd` with no options
int	cmd_pwd(t_ast *ast, t_cmd_prop *prop)
{
	char	dir[PATH_MAX + 1];

	if (getcwd(dir, sizeof(dir)) == NULL)
		return_prt_err(EXIT_FAIL, "minishell", "pwd", NULL);
	printf("%s\n", dir);
	return (EXIT_OK);
}

// @brief `exit` with no options
int	cmd_exit(t_ast *ast, t_cmd_prop *prop)
{
	int	status;

	if (prop->size != 1 && prop->size != 2)
		return_prt_err(EXIT_FAIL, "minishell", "exit", "too many arguments");
	status = EXIT_OK;
	if (prop->size == 2 && (ft_strncmp(ast->tokens[prop->start + 1], "0",
				2) != 0 && ft_strncmp(ast->tokens[prop->start + 1], "+0",
				3) != 0 && ft_strncmp(ast->tokens[prop->start + 1], "-0",
				3) != 0))
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
	exit_with_err(&ast, status, NULL);
	return (status);
}
