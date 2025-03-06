/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler_supp2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 07:50:41 by Xifeng            #+#    #+#             */
/*   Updated: 2025/03/06 10:50:45 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../libs/libft/libft.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

bool		is_relative_or_absolute_cmd(char *cmd);
char		**get_path(t_ast *ast);
void		free_path(char ***path);
char		*ms_strjoin(t_ast *ast, char *path, char *cmd);

// @brief try to parse the full cmd from env.
//
// @param ast: the pointer to ast tree.
// @param prop: the pointer to property of node.
// @return if there is existing file.
static bool	parse_full_cmd(t_ast *ast, t_cmd_prop *prop)
{
	int		i;
	char	**path;
	char	*full_cmd;

	i = 0;
	path = get_path(ast);
	if (!path)
		return (false);
	while (path[i])
	{
		full_cmd = ms_strjoin(ast, path[i], ast->tokens[prop->start]);
		if (access(full_cmd, F_OK) == 0)
		{
			prop->full_cmd = full_cmd;
			free_path(&path);
			return (true);
		}
		free(full_cmd);
		++i;
	}
	free_path(&path);
	return (false);
}

// @brief check if the full cmd is valid.
//
// @param ast: the pointer to the tree.
// @param prop: the property of node.
// @return if the cmd is valid.
static int	check_cmd(t_ast *ast, t_cmd_prop *prop)
{
	struct stat	buf;

	if (access(prop->full_cmd, F_OK) < 0)
		return (return_prt_err(EXIT_CMD_ERR, "minishell",
				ast->tokens[prop->start], NULL));
	if (access(prop->full_cmd, X_OK) < 0)
		return (return_prt_err(EXIT_EXEC_ERR, "minishell",
				ast->tokens[prop->start], NULL));
	stat(prop->full_cmd, &buf);
	if (S_ISDIR(buf.st_mode))
		return (return_prt_err(EXIT_EXEC_ERR, "minishell",
				ast->tokens[prop->start], "Is a directory"));
	return (EXIT_OK);
}

// @brief generate the argv for cmd.
//
// @param ast: the pointer to ast.
// @param prop: the property of a node.
void	generate_argv(t_ast *ast, t_cmd_prop *prop)
{
	int	i;

	prop->argv = ft_calloc(prop->size + 1, sizeof(char *));
	if (!(prop->argv))
		exit_with_err(&ast, EXIT_FAIL, "minishell: malloc");
	i = 0;
	while (i < prop->size)
	{
		prop->argv[i] = ast->tokens[prop->start + i];
		++i;
	}
}

// @brief to parse the full cmd and check if it is valid.
//
// @param ast: the pointer to ast.
// @param prop: the property of `cmd` node.
// @return the status.
int	parse_full_cmd_and_check(t_ast *ast, t_cmd_prop *prop)
{
	if (is_relative_or_absolute_cmd(ast->tokens[prop->start]))
	{
		prop->full_cmd = ft_strdup(ast->tokens[prop->start]);
		return (check_cmd(ast, prop));
	}
	if (parse_full_cmd(ast, prop))
		return (check_cmd(ast, prop));
	return (return_prt_err(EXIT_CMD_ERR, NULL, ast->tokens[prop->start],
			"command not found"));
}

// @brief close the envp.
//
// @param the pointer to envp.
void	close_envp(char ***envp)
{
	int	i;

	i = 0;
	if (!envp || !(*envp))
		return ;
	while ((*envp)[i])
		free((*envp)[i++]);
	free(*envp);
	*envp = NULL;
}
