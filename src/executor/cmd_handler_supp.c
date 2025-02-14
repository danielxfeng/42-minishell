/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler_supp.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 08:05:11 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/14 17:36:14 by Xifeng           ###   ########.fr       */
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
//
// @param the command to be checked.
// @return is the command empty?
bool	is_empty_cmd(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] != ' ')
			return (false);
		++i;
	}
	return (true);
}

// @brief to parse the PATH from env.
//
// @return the array of path.
char	**get_path(void)
{
	char	*path_str;
	char	**path;

	path_str = env_get("PATH");
	if (!path_str)
		return (NULL);
	path = ft_split(path_str, ':');
	if (!path)
		return (NULL);
	return (path);
}

// @brief free the array of path.
//
// @param path: the pointer to path.
void	free_path(char ***path)
{
	int	i;

	if (!path || *(*path))
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
