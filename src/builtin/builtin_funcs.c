/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:25:11 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/22 20:38:53 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int			check_option(t_ast *ast, t_cmd_prop *prop);

// @brief help `echo` to print out the parameters.
//
// @param ast: the pointer to ast tree.
// @param prop: the property of ast node.
// @param no_line_break: the flag of the line break sign in the end.
static void	echo_helper(t_ast *ast, t_cmd_prop *prop, bool no_line_break)
{
	int	i;

	i = 1;
	if (no_line_break)
		i = 2;
	while (i < prop->size)
	{
		if (ms_strcmp("$?", ast->tokens[prop->start + i]) == 0)
			printf("%d", ast->prev_status);
		else
			printf("%s", ast->tokens[prop->start + i]);
		if (i + 1 < prop->size)
			printf(" ");
		++i;
	}
	if (!no_line_break)
		printf("\n");
}

// @brief `echo` with option `-n`
// check 'man' page for more information.
// argc: prop->size;
// argv: ast->tokens[prop->start];
int	cmd_echo(t_ast *ast, t_cmd_prop *prop)
{
	int		status;
	bool	no_line_break;

	if (prop->size == 1)
	{
		printf("\n");
		return (EXIT_OK);
	}
	no_line_break = false;
	if (prop->size >= 2 && ms_strcmp(ast->tokens[prop->start + 1], "-n") == 0)
		no_line_break = true;
	else
	{
		status = check_option(ast, prop);
		if (status != EXIT_OK)
			return (status);
	}
	echo_helper(ast, prop, no_line_break);
	return (EXIT_OK);
}

// @brief `cd` with only a relative or absolute path
// check 'man' page for more information.
// argc: prop->size;
// argv: ast->tokens[prop->start];
int	cmd_cd(t_ast *ast, t_cmd_prop *prop)
{
	int		status;
	char	*dir;

	status = check_option(ast, prop);
	if (status != EXIT_OK)
		return (status);
	if (prop->size > 2)
		return (return_prt_err(EXIT_FAIL, "minishell", "cd",
				"too many arguments"));
	if (prop->size == 1)
		dir = env_get(ast->env, "HOME");
	else
		dir = ft_strdup(ast->tokens[prop->start + 1]);
	if (!dir)
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	if (prop->size == 1 && ms_strcmp("", dir) == 0)
		return (return_prt_err(EXIT_FAIL, "minishell", "cd", "HOME not set"));
	if (chdir(dir) != 0)
	{
		free(dir);	
		return (return_prt_err(EXIT_FAIL, "minishell: cd",
				ast->tokens[prop->start + 1], NULL));
	}
	free(dir);
	return (EXIT_OK);
}

// @brief `pwd` with no options
// check 'man' page for more information.
// argc: prop->size;
// argv: ast->tokens[prop->start];
int	cmd_pwd(t_ast *ast, t_cmd_prop *prop)
{
	char	*buf;
	int		status;

	status = check_option(ast, prop);
	if (status != EXIT_OK)
		return (status);
	buf = getcwd(NULL, 0);
	if (!buf)
		return (return_with_err(INVALID_ERR_NO, EXIT_FAIL,
				"minishell: getcwd"));
	printf("%s\n", buf);
	free(buf);
	return (EXIT_OK);
}

// @brief `exit` with no options
// check 'man' page for more information.
// argc: prop->size;
// argv: ast->tokens[prop->start];
int	cmd_exit(t_ast *ast, t_cmd_prop *prop)
{
	int	status;

	status = -1;
	printf("exit\n");
	if (prop->size > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		status = EXIT_FAIL;
	}
	else if (prop->size == 1)
		status = EXIT_OK;
	else if (prop->size == 2)
	{
		if (!ms_atoi(ast->tokens[prop->start + 1], &status))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(ast->tokens[prop->start + 1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			status = EXIT_INVALID_OPTION;
		}
	}
	exit_with_err(&ast, status, NULL);
	return (status);
}
