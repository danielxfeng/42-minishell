/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_directory_funcs.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:45:18 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/06 21:18:46 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int			check_option(t_ast *ast, t_cmd_prop *prop);

// @brief help to set the env for `cd`.
// `PWD` and 'OLDPWD' are set after `cd`.
//
// @param ast: the pointer to tree.
// @param dir: the path.
// @return the value of env.
static void	env_set_helper(t_ast *ast, char *dir)
{
	char	*env_item;
	char	*buf;

	buf = env_get(ast->env, "PWD");
	if (!buf)
		exit_for_cd(ast, &dir);
	env_item = ft_strjoin("OLDPWD=", buf);
	free(buf);
	if (!env_item)
		exit_for_cd(ast, &dir);
	env_set(ast->env, env_item);
	free(env_item);
	env_item = ft_strjoin("PWD=", dir);
	if (!env_item)
		exit_for_cd(ast, &dir);
	env_set(ast->env, env_item);
	free(env_item);
}

// @brief help the 'cd' to handle the directory switch.
//
// @param ast: the pointer to ast tree.
// @param prop: the property of node.
// @param dir: the path to switch.
// @return the status code.
static int	cd_helper(t_ast *ast, t_cmd_prop *prop, char *dir)
{
	char	*buf;

	if (chdir(dir) != 0)
	{
		free(dir);
		return (return_prt_err(EXIT_FAIL, "minishell: cd",
				ast->tokens[prop->start + 1], NULL));
	}
	buf = getcwd(NULL, 0);
	if (!buf)
	{
		free(dir);
		return (return_prt_err(EXIT_OK,
				"cd: error retrieving current directory",
				"getcwd: cannot access parent directories",
				"No such file or directory"));
	}
	env_set_helper(ast, buf);
	free(buf);
	free(dir);
	return (EXIT_OK);
}

// @brief `cd` with only a relative or absolute path
// check 'man' page for more information.
//
// 1 error handling.
// 2 if `cd` is without a argument, we `cd` to `$HOME`.
// 3 if `cd` failed, return error.
// 4 handle a corner case that the target directory is deleted.
// 5 set the env `PWD` and `OLDPWD`. 
//
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

// @brief get the current directory.
//
// We tried to get the directory from env first,
// `getcwd` is called only when `$PWD` is not available.
//
// @param ast: the pointer to ast tree.
// @return the string of pwd, the caller should free it.
static char	*get_pwd(t_ast *ast)
{
	char	*buf;

	buf = env_get(ast->env, "PWD");
	if (*buf && ft_strlen(buf) > 0)
		return (buf);
	free(buf);
	buf = getcwd(NULL, 0);
	if (!buf)
		return (NULL);
	return (buf);
}

// @brief `pwd` with no options
//
// The print need to be executed in sub-process for closing the pipe.
//
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
	buf = get_pwd(ast);
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
