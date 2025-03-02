/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler_supp.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 08:05:11 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/02 16:59:42 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>

// @brief is the cmd with absolute path, or relative path?
//
// @param cmd: the command.
// @return if the cmd is with absolute path or relative path.
bool	is_relative_or_absolute_cmd(char *cmd)
{
	if (ft_strncmp("/", cmd, 1) == 0 || ft_strncmp("./", cmd, 2) == 0
		|| ft_strncmp("../", cmd, 3) == 0)
		return (true);
	return (false);
}

// @brief is this a empty command?
// This function has side effect.
// Will perform the empty cmd.
//
// @param the command to be checked.
// @param status: the status code.
// @return is the command empty?
bool	is_empty_cmd(t_ast *ast, t_cmd_prop *prop, char *cmd, int *status)
{
	int	i;

	if (!cmd[0] && !(prop->is_quote))
	{
		if (prop->size > 1)
		{
			++(prop->start);
			--(prop->size);
			return (false);
		}
		*status = EXIT_OK;
		return (true);
	}
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] != ' ' && cmd[i] != '\t' )
			return (false);
		++i;
	}
	return_prt_err(EXIT_CMD_ERR, NULL, ast->tokens[prop->start],
		"command not found");
	*status = EXIT_CMD_ERR;
	return (true);
}

// @brief to parse the PATH from env.
//
// @param ast: the pointer to ast tree.
// @return the array of path.
char	**get_path(t_ast *ast)
{
	char	*path_str;
	char	**path;

	path_str = env_get(ast->env, "PATH");
	if (!path_str)
		return (NULL);
	path = ft_split(path_str, ':');
	free(path_str);
	return (path);
}

// @brief free the array of path.
//
// @param path: the pointer to path.
void	free_path(char ***path)
{
	int	i;

	if (!path || !(*path))
		return ;
	i = 0;
	while ((*path)[i])
	{
		free((*path)[i]);
		++i;
	}
	free(*path);
	*path = NULL;
}

// @brief join the path and cmd
//
// @param path: the path.
// @param cmd: the cmd.
// @return (path/cmd);
char	*ms_strjoin(t_ast *ast, char *path, char *cmd)
{
	char	*joined;

	if (!*path)
		return (cmd);
	joined = ft_calloc((ft_strlen(path) + ft_strlen(cmd) + 2), sizeof(char));
	if (!joined)
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	ft_memcpy(joined, path, ft_strlen(path));
	joined[ft_strlen(path)] = '/';
	ft_memcpy(joined + ft_strlen(path) + 1, cmd, ft_strlen(cmd));
	return (joined);
}
