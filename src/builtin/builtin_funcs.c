/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:25:11 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/04 15:36:53 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int			check_option(t_ast *ast, t_cmd_prop *prop);
int			cd_helper(t_ast *ast, t_cmd_prop *prop, char *dir);
int			return_process_res(int status);
void		echo_helper(t_ast *ast, t_cmd_prop *prop, bool no_line_break);
void		empty_echo_helper(t_ast *ast, t_cmd_prop *prop);

static void	handle_sub_proc(t_ast *ast, int pid)
{
	if (pid < 0)
		close_builtin_proc(&ast, EXIT_FAIL, "minishell: fork");
}

// @brief `echo` with option `-n`
// check 'man' page for more information.
// argc: prop->size;
// argv: ast->tokens[prop->start];
int	cmd_echo(t_ast *ast, t_cmd_prop *prop)
{
	int		status;
	int		pid;
	bool	no_line_break;

	status = 0;
	pid = fork();
	handle_sub_proc(ast, pid);
	if (pid == 0)
	{
		empty_echo_helper(ast, prop);
		no_line_break = false;
		if (prop->size >= 2 && ms_strcmp(ast->tokens[prop->start + 1],
				"-n") == 0)
			no_line_break = true;
		else
		{
			status = check_option(ast, prop);
			if (status != EXIT_OK)
				close_builtin_proc(&ast, status, NULL);
		}
		echo_helper(ast, prop, no_line_break);
		close_builtin_proc(&ast, status, NULL);
	}
	waitpid(pid, &status, 0);
	return (return_process_res(status));
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
	{
		free(dir);
		return (return_prt_err(EXIT_FAIL, "minishell", "cd", "HOME not set"));
	}
	return (cd_helper(ast, prop, dir));
}

// @brief `pwd` with no options
// check 'man' page for more information.
// argc: prop->size;
// argv: ast->tokens[prop->start];
int	cmd_pwd(t_ast *ast, t_cmd_prop *prop)
{
	char	*buf;
	int		status;
	int		pid;

	status = check_option(ast, prop);
	if (status != EXIT_OK)
		return (status);
	buf = getcwd(NULL, 0);
	if (!buf)
		return (return_with_err(INVALID_ERR_NO, EXIT_FAIL,
				"minishell: getcwd"));
	pid = fork();
	if (pid < 0)
		close_builtin_proc(&ast, EXIT_FAIL, "minishell: fork");
	if (pid == 0)
	{
		printf("%s\n", buf);
		free(buf);
		close_builtin_proc(&ast, EXIT_OK, NULL);
	}
	free(buf);
	waitpid(pid, NULL, 0);
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
		return (status);
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
